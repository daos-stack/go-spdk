// cmpout -tags=use_go_run

// Copyright 2009 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

// +build test_run

package main

import "fmt"
import "../spdk"

func main() {
	if err := spdk.InitSPDKEnv(); err != nil {
		fmt.Printf("Unable to initialise SPDK env (%s)\n", err)
	}
	if err := spdk.NVMeDiscover(); err != nil {
		fmt.Printf("Unable to discover NVMe devices (%s)\n", err)
	}
}