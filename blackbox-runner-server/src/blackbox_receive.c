#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "blackbox_receive.h"
#include "blackbox_data.h"
#include "progress_bar.h"

int print_packet_information(const blackbox_part_info *p_part_info)
{
    assert(p_part_info != NULL);

    printf("[SERVER][INFO] Packet info - Total Size %u,"
           "Packet Size %u,"
           "Bin Count %u,"
           "Bin Packet Len %u,"
           "Transfer Status %u\n",
           p_part_info->total_size,
           p_part_info->packet_size,
           p_part_info->bin_count,
           p_part_info->bin_packet.bin_packet_len,
           p_part_info->transfer_status);
}

FILE *open_blackbox_file(const char *p_blackbox_file_name)
{
    assert(p_blackbox_file_name != NULL);
    FILE *file = fopen(p_blackbox_file_name, "wb");
    if (!file)
    {
        printf("[SERVER][ERROR] Failed to open file %s\n", p_blackbox_file_name);
        return NULL;
    }
    return file;
}

int close_blackbox_file(FILE *p_file)
{
    assert(p_file != NULL);

    printf("[SERVER][INFO] Closing file..\n");
    if (0 != fclose(p_file))
    {
        printf("[SERVER][ERROR] Error occured while trying to close the file..\n");
        return 1;
    }
    return 0;
}

size_t add_packet_to_blackbox_file(FILE *p_blackbox_file, const blackbox_part_info *p_part_info)
{
    assert(p_blackbox_file != NULL);
    assert(p_part_info != NULL);

    print_progress("[SERVER][INFO] Downloading", p_part_info->bin_count, p_part_info->total_size);
    return (fwrite(p_part_info->bin_packet.bin_packet_val, 1, p_part_info->bin_packet.bin_packet_len, p_blackbox_file));
}