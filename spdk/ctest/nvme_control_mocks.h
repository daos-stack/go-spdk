/**
 * This is a mock of spdk functions used in nvme_control.c
 * Function definitions are in:
 * spdk/env.h
 * spdk/nvme.h
 *
 * All functions are wrapped with a prefix: '__wrap'
 *
 */

/**
 * ==============================
 * nvme_control private functions
 * ==============================
 */
void __wrap_register_ns(struct spdk_nvme_ctrlr *ctrlr, struct spdk_nvme_ns *ns);
bool __wrap_probe_cb(void *cb_ctx, const struct spdk_nvme_transport_id *trid,
		struct spdk_nvme_ctrlr_opts *opts);
void __wrap_attach_cb(void *cb_ctx, const struct spdk_nvme_transport_id *trid,
		struct spdk_nvme_ctrlr *ctrlr,
		const struct spdk_nvme_ctrlr_opts *opts);
struct ret_t * __wrap_init_ret(void);
int __wrap_check_size(int written, int max, char *msg, struct ret_t *ret);
int __wrap_set_pci_addr(struct spdk_nvme_ctrlr *ctrlr, char *ctrlr_pci_addr,
		size_t size, struct ret_t *ret);
void __wrap_collect(struct ret_t *ret);
void __wrap_cleanup(void);

/**
 * ===============
 *  SPDK Functions
 *  ==============
 */

/**
 *Get the identify controller data as defined by the NVMe specification
 *
 */
const struct spdk_nvme_ctrlr_data * __wrap_spdk_nvme_ctrlr_get_data(struct spdk_nvme_ctrlr *ctrlr);

/**
 * Determine whether a namespace is active.
 *
 */
bool __wrap_spdk_nvme_ns_is_active(struct spdk_nvme_ns *ns);

/**
 * Get the namespace ID from given namespace handle
 *
 */
uint32_t __wrap_spdk_nvme_ns_get_id(struct spdk_nvme_ns *ns);

/**
 * Get the number of namespaces for the given NVMe controller
 *
 */
uint32_t __wrap_spdk_nvme_ctrlr_get_num_ns(struct spdk_nvme_ctrlr *ctrlr);

/**
 * Get handle to a namespace for the given controller
 *
 */
struct spdk_nvme_ns * __wrap_spdk_nvme_ctrlr_get_ns(struct spdk_nvme_ctrlr *ctrlr, uint32_t ns_id);

/**
 * Get the PCI device of a given NVME controller.
 *
 */
struct spdk_pci_device * __wrap_spdk_nvme_ctrlr_get_pci_device(struct spdk_nvme_ctrlr *ctrlr);

/**
 * Get the full DomainBDF address of a PCI device
 *
 */
struct spdk_pci_addr __wrap_spdk_pci_device_get_addr(struct spdk_pci_device *dev);

/**
 * Convert a struct spdk_pci_addr to a string
 *
 */
int __wrap_spdk_pci_addr_fmt(char *bdf, size_t sz, const struct spdk_pci_addr *addr);

/**
 * Get the size, in bytes, of the given namespace
 *
 */
uint64_t __wrap_spdk_nvme_ns_get_size(struct spdk_nvme_ns *ns);

/**
 * Detach specified device returned by spdk_nvme_probe()'s attach_cb from the NVMe driver
 *
 */
int __wrap_spdk_nvme_detach(struct spdk_nvme_ctrlr *ctrlr);

/**
 * Enumerate the bus indicated by the transport ID and attach the userspace NVMe driver 
 * to each device found if desired
 *
 */
int __wrap_spdk_nvme_probe(const struct spdk_nvme_transport_id *trid, void *cb_ctx,
		spdk_nvme_probe_cb probe_cb, 
		spdk_nvme_attach_cb attach_cb, 
		spdk_nvme_remove_cb remove_cb);

/**
 * Convert a string representation of a PCI address into a struct spdk_pci_addr.
 *
 */
int __wrap_spdk_pci_addr_parse(struct spdk_pci_addr *addr, const char *bdf);

/**
 * Compare two PCI addresses
 *
 */
int __wrap_spdk_pci_addr_compare(const struct spdk_pci_addr *a1, const struct spdk_pci_addr *a2);

/**
 * Allocate a pinned memory buffer with the given size and alignment
 *
 */
void * __wrap_spdk_dma_zmalloc(size_t size, size_t align, uint64_t *phys_addr);

/**
 * Free a memory buffer previously allocated, for example from spdk_dma_zmalloc()
 *
 */
void __wrap_spdk_dma_free(void *buf);

/**
 * Download a new firmware image
 *
 */
int __wrap_spdk_nvme_ctrlr_update_firmware(struct spdk_nvme_ctrlr *ctrlr,
		void *payload, uint32_t size, int slot,
		enum spdk_nvme_fw_commit_action commit_action,
		struct spdk_nvme_status *completion_status);

/**
 * Format NVM
 *
 */
int __wrap_spdk_nvme_ctrlr_format(struct spdk_nvme_ctrlr *ctrlr, uint32_t nsid,
		struct spdk_nvme_format *format);
