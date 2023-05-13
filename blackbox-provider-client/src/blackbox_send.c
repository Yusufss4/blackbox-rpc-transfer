#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> // stat
#include <assert.h>

#include "blackbox_data.h"
#include "blackbox_send.h"

#include "progress_bar.h"

static unsigned int get_size_of_blackbox(FILE *fp)
{
    struct stat statbuf;

    int fd = fileno(fp);
    if (fd == -1)
    {
        printf("[CLIENT][ERROR] Handle error of file descriptor.\n");
        exit(EXIT_FAILURE);
    }

    if (fstat(fd, &statbuf) == -1)
    {
        printf("[CLIENT][ERROR] Failed to get the size of the file.\n");
        exit(EXIT_FAILURE);
    }

    return statbuf.st_size;
}

static int send_blackbox_packet(CLIENT *client, const blackbox_part_info *p_part_info)
{
    int *result = send_blackbox_1((blackbox_part_info *)p_part_info, client);
    if (result == NULL)
    {
        printf("[CLIENT][ERROR]: RCP Failed while trying to send a black packet.\n");
        return 1;
    }
    return *result;
}

static int release_resources_taken_for_blackbox_transfer(char *p_buffer, FILE *file)
{
    free(p_buffer);
    fclose(file);
}

static int print_packet_information(const blackbox_part_info *p_part_info)
{
    printf("[CLIENT] Packet info - Total Size %u,"
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

int send_blackbox(CLIENT *client, const char *p_blackbox_path, size_t packet_size)
{
    assert(client != NULL);
    assert(p_blackbox_path != NULL);
    assert(packet_size > 0);

    FILE *p_blackbox_file = fopen(p_blackbox_path, "rb");
    if (!p_blackbox_file)
    {
        printf("[CLIENT][ERROR] Failed to open file %s\n", p_blackbox_path);
        return 1;
    }

    blackbox_part_info part_info;
    part_info.transfer_status = BLACKBOX_TRANSFER_START;
    part_info.total_size = get_size_of_blackbox(p_blackbox_file);
    part_info.packet_size = packet_size;
    part_info.bin_count = 0;
    int num_packets = (part_info.total_size + packet_size - 1) / packet_size;
    size_t last_packet_size = part_info.total_size % packet_size;

    printf("[CLIENT][INFO] Blackbox total size : %u bytes.\n", part_info.total_size);
    printf("[CLIENT][INFO] Blackbox number of packets divided : %d packets.\n", num_packets);
    printf("[CLIENT][INFO] Blackbox last packet size  %lu bytes.\n", last_packet_size);

    // Allocate a buffer for each packet, read data from the file, and send it.
    uint8_t *p_buffer = (uint8_t *)malloc(packet_size * sizeof(uint8_t));

    print_progress("[CLIENT] Uploading", 0, part_info.total_size);

    for (int i = 0; i < num_packets; i++)
    {
        size_t bytes_read = 0;
        // if packet is last packet
        if (i == (num_packets - 1))
        {
            bytes_read = fread(p_buffer, 1, last_packet_size, p_blackbox_file);
        }
        else
        {
            bytes_read = fread(p_buffer, 1, packet_size, p_blackbox_file);
        }

        // Send the packet.
        part_info.bin_packet.bin_packet_val = p_buffer;
        part_info.bin_packet.bin_packet_len = bytes_read;
        part_info.bin_count += bytes_read;

        // print_packet_information(&part_info);
        if (0 != send_blackbox_packet(client, &part_info))
        {
            printf("[CLIENT][ERROR] Failed to blackbox packet. \n");
            release_resources_taken_for_blackbox_transfer(p_buffer, p_blackbox_file);
            return 1;
        }

        print_progress("[CLIENT] Uploading", part_info.bin_count, part_info.total_size);

        part_info.transfer_status = BLACKBOX_TRANSFER_PROGRESS;
        // Clear the buffer for the next packet.
        memset(p_buffer, 0, packet_size);
    }

    release_resources_taken_for_blackbox_transfer(p_buffer, p_blackbox_file);
    return 0;
}
