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

#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "spdk/stdinc.h"
#include "spdk/nvme.h"
#include "spdk/env.h"

#include "nvme_control.h"
#include "nvme_control_common.h"

int __real_close(int fildes);
int __real_open(const char *path, int oflags);
int __real_fstat(int fildes, struct stat *buf);

/**
 * ==============================
 * nvme_control mock functions
 * ==============================
 */

struct ret_t * __wrap_init_ret(void)
{
    return mock_ptr_type(struct ret_t *);
}

void __wrap_collect(struct ret_t *ret)
{}

int __wrap_get_controller(char *addr, struct ctrlr_entry *ctrlr_entry,
        struct ret_t *ret)
{
    return mock_type(int);
}

/**
 * ==============================
 * file operations mock functions
 * ==============================
 */
int __wrap_open(const char *path, int oflags)
{
    return mock_type(int);
}

ssize_t __wrap_read(int fildes, void *buf, size_t nbytes)
{
    return mock_type(ssize_t);
}

int __wrap_fstat(int fildes, struct stat *buf)
{
    return __real_fstat(fildes, buf);
}

int __wrap_close(int fildes)
{
    return __real_close(fildes);
}

/**
 * ===================
 * SPDK mock functions
 * ===================
 */

int __wrap_spdk_nvme_ctrlr_update_firmware(struct spdk_nvme_ctrlr *ctrlr,
        void *payload,
        uint32_t size,
        int slot,
        enum spdk_nvme_fw_commit_action commit_action,
        struct spdk_nvme_status * completion_status)
{
    completion_status->sct = mock_type(int);
    completion_status->sc = mock_type(int);
    return mock_type(int);
}

void *__wrap_spdk_dma_zmalloc(size_t size, size_t align, uint64_t phys_addr)
{
    return mock_ptr_type(void *);
}

void __wrap_spdk_dma_free(void *buff)
{
    (void) buff; /*unused*/
}

/**
 * ===================
 * Test functions
 * ===================
 */
static void test_nvme_fwupdate_need_reset(void **state)
{
    (void)  state; /*unused*/
    struct  ret_t *rv, *ret;
    int     fd = -1;
    int     fw_image_val = 5;
    void    *fw_image = &fw_image_val;
    char    *pci_addr = "test:81:00.0";
    char    *pci_addr_path = "ctest/fwupdate_test.txt";

    /* ret_t values for mock init_ret */
    if(ret == NULL){
        ret = test_malloc(sizeof(struct ret_t));
        ret->rc = 0;
        ret->ctrlrs = NULL;
        ret->nss = NULL;
        snprintf(ret->err, sizeof(ret->err), "none");
    }

    if(g_controllers == NULL) {
        g_controllers = test_malloc(sizeof(struct ctrlr_entry));
        g_controllers->ctrlr = NULL;
        g_controllers->tr_addr = NULL;
        g_controllers->next = NULL;
    }

    //Open file for testing
    fd = __real_open(pci_addr_path, O_RDWR);
    assert_return_code(fd, 0);

    /* Define return values */
    will_return(__wrap_init_ret, &ret);
    will_return(__wrap_get_controller, 0);
    will_return(__wrap_read, 0);
    will_return(__wrap_open, fd);
    will_return(__wrap_spdk_dma_zmalloc, &fw_image);

    /**
    * ENXIO = 6
    * SPDK_NVME_SCT_COMMAND_SPECIFIC = 1
    * SPDK_NVME_SC_FIRMWARE_REQ_CONVENTIONAL_RESET = 11
    *
    * Pass in values to set status.sct, status.sc and rc
    **/
    will_return(__wrap_spdk_nvme_ctrlr_update_firmware, 1);
    will_return(__wrap_spdk_nvme_ctrlr_update_firmware, 11);
    will_return(__wrap_spdk_nvme_ctrlr_update_firmware, -6);

    rv = nvme_fwupdate(pci_addr, pci_addr_path, 1);
    if (rv != NULL) {
        printf("nvme_fwupdate() return code: %d\n", rv->rc);
        printf("nvme_fwupdate() error string: %s\n", rv->err);
    }

    test_free(g_controllers);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_nvme_fwupdate_need_reset),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}