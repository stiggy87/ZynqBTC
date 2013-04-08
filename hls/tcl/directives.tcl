############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 2013 Xilinx Inc. All rights reserved.
############################################################
#set_directive_interface -mode ap_memory sha256_top data
#set_directive_interface -mode ap_memory sha256_top hash
set_directive_interface -mode ap_fifo -depth 64 sha256_top data 
set_directive_interface -mode ap_vld -register sha256_top hash
set_directive_interface -mode ap_ctrl_none sha256_top
#set_directive_interface -mode ap_memory sha256_top ctx.in_data
#set_directive_interface -mode ap_bus -register sha256_top ctx.datalen
#set_directive_interface -mode ap_bus -register sha256_top ctx.bitlen
#set_directive_interface -mode ap_memory sha256_top ctx.state

#set_directive_resource -core RAM_2P_BRAM sha256_top ctx.in_data
#set_directive_resource -core RAM_2P_BRAM sha256_top ctx.state
set_directive_resource -core RAM_2P_BRAM sha256_top hash
set_directive_resource -core RAM_2P_BRAM sha256_top data