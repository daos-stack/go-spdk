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

#include <cmocka.h>

#include "spdk/stdinc.h"
#include "spdk/nvme.h"
#include "spdk/env.h"

#include "nvme_control.h"
#include "nvme_control_mocks.h"

/**
 * ==============================
 * nvme_control private functions
 * ==============================
 */

struct ret_t * __wrap_init_ret(void)
{
	return mock_type(ret_t *);
}

void __wrap_cleanup(void)
{
	struct ns_entry		*ns_entry;
	struct ctrlr_entry	*ctrlr_entry;

	ns_entry = mock_type(ns_entry *);
	ctrlr_entry = mock_type(ctrlr_entry);

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
}

void __wrap_collect(struct ret_t *ret)
{
	check_expected_ptr(ret);
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
	/* Check parameters */
	check_expected_ptr(trid);
	check_expected_ptr(cb_trix);
	check_expected(probe_cb);
	check_expected(attach_cb);
	check_expected(remove_cb);

	return mock_type(int);
}
