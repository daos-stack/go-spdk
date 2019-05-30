//
// (C) Copyright 2018-2019 Intel Corporation.
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

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "spdk/stdinc.h"
#include "spdk/nvme.h"
#include "spdk/env.h"

#include "nvme_control.h"

struct ctrlr_entry {
	struct spdk_nvme_ctrlr	*ctrlr;
	const char		*tr_addr;
	struct ctrlr_entry	*next;
};

struct ns_entry {
	struct spdk_nvme_ctrlr	*ctrlr;
	struct spdk_nvme_ns	*ns;
	struct ns_entry		*next;
	struct spdk_nvme_qpair	*qpair;
};

/**
 * ==============================
 * nvme_control private functions
 * ==============================
 */

struct ret_t * __wrap_init_ret(void)
{
	printf("Mock init_ret() runnning...");
	return mock_ptr_type(struct ret_t *);
}

void __wrap_cleanup(void)
{
	printf("Mock cleanup() running...");
	struct ns_entry		*ns_entry;
	struct ctrlr_entry	*ctrlr_entry;

	//ns_entry = g_namespaces;
	//ctrlr_entry = g_controllers;

	/*
	while (ns_entry) {
		struct ns_entry *next = ns_entry->next;

		test_free(ns_entry);
		ns_entry = next;
	}

	while (ctrlr_entry) {
		struct ctrlr_entry *next = ctrlr_entry->next;

		test_free(ctrlr_entry);
		ctrlr_entry = next;
	}
	*/
}

void __wrap_collect(struct ret_t *ret)
{
	printf("Mock collect() running...");
	struct ns_entry				*ns_entry;
	struct ctrlr_entry			*ctrlr_entry;
	const struct spdk_nvme_ctrlr_data	*cdata;
	int					written;

	//ns_entry = g_namespaces;
	//ctrlr_entry = g_controllers;

	/*
	while (ns_entry) {
		struct ns_t *ns_tmp;

		ns_tmp = test_malloc(sizeof(struct ns_t));

		if (ns_tmp == NULL) {
			snprintf(ret->err, sizeof(ret->err), "ns_t malloc");
			ret->rc = -ENOMEM;
			return;
		}

		cdata = spdk_nvme_ctrlr_get_data(ns_entry->ctrlr);

		ns_tmp->id = spdk_nvme_ns_get_id(ns_entry->ns);
		// capacity in GBytes
		ns_tmp->size = spdk_nvme_ns_get_size(ns_entry->ns) / \
			       NVMECONTROL_GBYTE_BYTES;

		if (set_pci_addr(
			ns_entry->ctrlr, ns_tmp->ctrlr_pci_addr,
			sizeof(ns_tmp->ctrlr_pci_addr), ret) != 0) {

			return;
		}

		ns_tmp->next = ret->nss;
		ret->nss = ns_tmp;

		ns_entry = ns_entry->next;
	}

	while (ctrlr_entry) {
		struct ctrlr_t *ctrlr_tmp;

		ctrlr_tmp = test_malloc(sizeof(struct ctrlr_t));

		if (ctrlr_tmp == NULL) {
			perror("ctrlr_t malloc");
			ret->rc = -ENOMEM;
			return;
		}

		cdata = spdk_nvme_ctrlr_get_data(ctrlr_entry->ctrlr);

		written = snprintf(
			ctrlr_tmp->model, sizeof(ctrlr_tmp->model),
			"%-20.20s", cdata->mn
		);
		if (check_size(
			written, sizeof(ctrlr_tmp->model),
			"model truncated", ret) != 0) {

			return;
		}

		written = snprintf(
			ctrlr_tmp->serial, sizeof(ctrlr_tmp->serial),
			"%-20.20s", cdata->sn
		);
		if (check_size(
			written, sizeof(ctrlr_tmp->serial),
			"serial truncated", ret) != 0) {

			return;
		}

		written = snprintf(
			ctrlr_tmp->fw_rev, sizeof(ctrlr_tmp->fw_rev),
			"%s", cdata->fr
		);
		if (check_size(
			written, sizeof(ctrlr_tmp->fw_rev),
			"firmware revision truncated", ret) != 0) {

			return;
		}

		if (set_pci_addr(
			ctrlr_entry->ctrlr, ctrlr_tmp->pci_addr,
			sizeof(ctrlr_tmp->pci_addr), ret) != 0) {

			return;
		}

		// cdata->cntlid is not unique per host, only per subsystem
		ctrlr_tmp->next = ret->ctrlrs;
		ret->ctrlrs = ctrlr_tmp;

		ctrlr_entry = ctrlr_entry->next;
	}

	ret->rc = 0;
	*/
}

/**
 * ==============
 * SPDK Functions
 * ==============
 */

int __wrap_spdk_nvme_probe(
		const struct spdk_nvme_transport_id *trid, 
		void *cb_ctx, 
		spdk_nvme_probe_cb probe_cb, 
		spdk_nvme_attach_cb attach_cb, 
		spdk_nvme_remove_cb remove_cb)
{
	printf("Mock spdk_nvme_probe() running...");

	/* Check parameters */
	//check_expected(probe_cb);
	//check_expected(attach_cb);

	/* Return integer */
	return mock_type(int);
}
