/* Gearman server and library
 * Copyright (C) 2008 Brian Aker, Eric Day
 * All rights reserved.
 *
 * Use and distribution licensed under the BSD license.  See
 * the COPYING file in the parent directory for full text.
 */

/**
 * @file
 * @brief Packet Declarations
 */

#ifndef __GEARMAN_PACKET_H__
#define __GEARMAN_PACKET_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Command information array.
 * @ingroup gearman_constants
 */
extern GEARMAN_API
gearman_command_info_st gearman_command_info_list[GEARMAN_COMMAND_MAX];

/**
 * @addtogroup gearman_packet Packet Declarations
 * @ingroup gearman
 *
 * This is a low level interface for gearman packet. This is used internally
 * internally by both client and worker interfaces (or more specifically, tasks
 * and jobs), so you probably want to look there first. This is usually used to
 * write lower level clients, workers, proxies, or your own server.
 *
 * @{
 */

/**
 * @ingroup gearman_packet
 */
struct gearman_packet_st
{
  struct {
    bool allocated:1;
    bool complete:1;
    bool free_data:1;
  } options;
  gearman_magic_t magic;
  gearman_command_t command;
  uint8_t argc;
  size_t args_size;
  size_t data_size;
  gearman_state_st *gearman;
  gearman_packet_st *next;
  gearman_packet_st *prev;
  uint8_t *args;
  const void *data;
  uint8_t *arg[GEARMAN_MAX_COMMAND_ARGS];
  size_t arg_size[GEARMAN_MAX_COMMAND_ARGS];
  uint8_t args_buffer[GEARMAN_ARGS_BUFFER_SIZE];
};


/**
 * Free a packet structure.
 *
 * @param[in] packet Structure previously initialized with
 *   gearman_add_packet() or gearman_add_packet_args().
 */
GEARMAN_API
void gearman_packet_free(gearman_packet_st *packet);

/**
 * Add an argument to a packet.
 */
GEARMAN_API
gearman_return_t gearman_packet_add_arg(gearman_packet_st *packet,
                                        const void *arg, size_t arg_size);

/**
 * Pack header.
 */
GEARMAN_API
gearman_return_t gearman_packet_pack_header(gearman_packet_st *packet);

/**
 * Unpack header.
 */
GEARMAN_API
gearman_return_t gearman_packet_unpack_header(gearman_packet_st *packet);

/**
 * Pack packet into output buffer.
 */
GEARMAN_API
size_t gearman_packet_pack(const gearman_packet_st *packet, gearman_con_st *con,
                           void *data, size_t data_size,
                           gearman_return_t *ret_ptr);

/**
 * Unpack packet from input data.
 */
GEARMAN_API
size_t gearman_packet_unpack(gearman_packet_st *packet, gearman_con_st *con,
                             const void *data, size_t data_size,
                             gearman_return_t *ret_ptr);

/**
 * Give allocated memory to packet. After this, the library will be responsible
 * for freeing the workload memory when the packet is destroyed.
 */
GEARMAN_API
void gearman_packet_give_data(gearman_packet_st *packet, const void *data,
                              size_t data_size);

/**
 * Take allocated data from packet. After this, the caller is responsible for
 * free()ing the memory.
 */
GEARMAN_API
void *gearman_packet_take_data(gearman_packet_st *packet, size_t *data_size);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __GEARMAN_PACKET_H__ */
