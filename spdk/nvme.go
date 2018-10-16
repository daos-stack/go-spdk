//
// (C) Copyright 2018 Intel Corporation.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// GOVERNMENT LICENSE RIGHTS-OPEN SOURCE SOFTWARE
// The Government's rights to use, modify, reproduce, release, perform, display,
// or disclose this software are subject to the terms of the Apache License as
// provided in Contract No. 8F-30005.
// Any reproduction of computer software, computer software documentation, or
// portions thereof marked with this legend must also reproduce the markings.
//

package spdk

// CGO_CFLAGS & CGO_LDFLAGS env vars can be used
// to specify additional dirs.

/*
#cgo CFLAGS: -I .
#cgo LDFLAGS: -L . -lnvme_control -lspdk

#include "stdlib.h"
#include "spdk/stdinc.h"
#include "spdk/nvme.h"
#include "spdk/env.h"
#include "include/nvme_control.h"
*/
import "C"

import (
	"fmt"
	"unsafe"
)

// Controller struct mirrors C.struct_ctrlr_t and
// describes a NVMe controller.
//
// TODO: populate implicitly using inner member:
// +inner C.struct_ctrlr_t
type Controller struct {
	ID      int32
	Model   string
	Serial  string
	PCIAddr string
	FWRev   string
}

// Namespace struct mirrors C.struct_ns_t and
// describes a NVMe Namespace tied to a controller.
//
// TODO: populate implicitly using inner member:
// +inner C.struct_ns_t
type Namespace struct {
	ID      int32
	Size    int32
	CtrlrID int32
}

// c2GoController is a private translation function
func c2GoController(ctrlr *C.struct_ctrlr_t) Controller {
	return Controller{
		ID:      int32(ctrlr.id),
		Model:   C.GoString(&ctrlr.model[0]),
		Serial:  C.GoString(&ctrlr.serial[0]),
		PCIAddr: C.GoString(&ctrlr.tr_addr[0]),
		FWRev:   C.GoString(&ctrlr.fw_rev[0]),
	}
}

// c2GoNamespace is a private translation function
func c2GoNamespace(ns *C.struct_ns_t) Namespace {
	return Namespace{
		ID:      int32(ns.id),
		Size:    int32(ns.size),
		CtrlrID: int32(ns.ctrlr_id),
	}
}

// processReturn parses return structs
func processReturn(retPtr *C.struct_ret_t, failLocation string) (
	[]Controller, []Namespace, error) {

	var ctrlrs []Controller
	var nss []Namespace

	defer C.free(unsafe.Pointer(retPtr))

	if retPtr.rc == 0 {
		ctrlrPtr := retPtr.ctrlrs
		for ctrlrPtr != nil {
			defer C.free(unsafe.Pointer(ctrlrPtr))
			ctrlrs = append(ctrlrs, c2GoController(ctrlrPtr))
			ctrlrPtr = ctrlrPtr.next
		}

		nsPtr := retPtr.nss
		for nsPtr != nil {
			defer C.free(unsafe.Pointer(nsPtr))
			nss = append(nss, c2GoNamespace(nsPtr))
			nsPtr = nsPtr.next
		}

		return ctrlrs, nss, nil
	}

	return nil, nil, fmt.Errorf(
		"%s failed, rc: %d, %s",
		failLocation,
		retPtr.rc,
		C.GoString(&retPtr.err[0]))
}

// NVMe is the interface that provides SPDK NVMe functionality.
type NVMe interface {
	// Discover NVMe controllers and namespaces
	Discover() ([]Controller, []Namespace, error)
	// Update NVMe controller firmware
	Update(ctrlrID int32, path string, slot int32)
	// Cleanup NVMe object references
	Cleanup()
}

// Nvme is an NVMe implementation.
type Nvme struct{}

// Discover calls C.nvme_discover which returns
// pointers to single linked list of ctrlr_t and ns_t structs.
// These are converted to slices of Controller and Namespace structs.
//
// \return ([]Controllers, []Namespace, nil) on success,
//         (nil, nil, error) otherwise
func (n *Nvme) Discover() ([]Controller, []Namespace, error) {
	failLocation := "NVMe Discover(): C.nvme_discover"

	if retPtr := C.nvme_discover(); retPtr != nil {
		return processReturn(retPtr, failLocation)
	}

	return nil, nil, fmt.Errorf(
		"%s unexpectedly returned NULL", failLocation)
}

// Update calls C.nvme_fwupdate to update controller firmware image.
//
// \ctrlrID Controller ID to perform update on
// \path Filesystem path to retrieve firmware image from
// \slot Firmware slot/register to upload to on controller
// \return ([]Controllers, []Namespace, nil) on success,
//         (nil, nil, error) otherwise
func (n *Nvme) Update(ctrlrID int32, path string, slot int32) (
	[]Controller, []Namespace, error) {

	csPath := C.CString(path)
	defer C.free(unsafe.Pointer(csPath))

	failLocation := "NVMe Update(): C.nvme_fwupdate"

	retPtr := C.nvme_fwupdate(C.uint(ctrlrID), csPath, C.uint(slot))
	if retPtr != nil {
		return processReturn(retPtr, failLocation)
	}

	return nil, nil, fmt.Errorf(
		"%s unexpectedly returned NULL", failLocation)
}

// Cleanup unlinks and detaches any controllers or namespaces.
func (n *Nvme) Cleanup() {
	C.nvme_cleanup()
}
