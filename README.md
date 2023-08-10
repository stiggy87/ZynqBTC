ZynqBTC
=======

A Bitcoin miner for the Zynq chip utilizing the Zedboard.

8/9/2023
***THIS IS ARCHIVED!*** I haven't updated this in years, and due to real-life things getting in the way, I am archiving this (after so many years of no updates). I'm glad some people still star it and fork it for their own purposes.

I do want to mention that the Vitis flow will make this easier.

Goal
====

The main goal of this is a proof-of-concept of making the Zedboard a fully functional Bitcoin miner for the open source community.

TODO
====

- Complete/Verify the SHA256 IP in HLS
- Create the Zynq PL utilizing the SHA256 IP as well as AXI4 FIFO Stream and AXI4 Interconnect (for multiple miners)
- Create the Arbiter in C/C++ for communication to the AXI4 Interconnect
- Configure the ARM to run Linux on Proc 0 and Arbiter on Proc 1
- Support the Stratum protocol (getwork will not be supported in the main branch. If getwork version is requested, a new branch will be created).
- Benchmark!!!!

Basic Design
============

The basic design is to utilize the ARM dual core processor to handle communication to a pool and act as an arbiter for the miners. Utilizing AXI4 capabilities, it will be able to communicate with the programmable logic (PL) with ease.

- Processor 0 - This is the main communication to the outside world. This processor will run Linux with Python/Perl scripts to handle datatransmission to/from pool and to/from arbiter
- Processor 1 - This is a bare-metal program which will communicate with Processor 0 for jobs, and the PL to distribute/read in work
- PL - Where the miners live.

Features
========

A fully functional Zynq Bitcoin Miner. Since not everything about it are currently defined below are specifications:
- Full AXI4 support -- The design will utilize the AXI4 specification for communication between the ARM to the PL and the subsystems on the PL
- Customizability -- The design will allow the end user to modify any point of the design, of course, within limitation of the WebPACK licensing for Zynq (this means some software/IP might not be available)

Key Concepts
============

- Use Vivado HLS to create and generate the SHA256 algorithm in an AXI4 FIFO format in order to get the best performance while limiting memory usage
- Limit HDL coding. (This means far fewer mistakes in the long run)
- "Productizing" - Each part of this system should be able to stand on its own, which allows anyone to create their own Zynq miner, or modify the current one with better results.
	- SHA256 - Standalone AXI4 Spec'd IP
	- Linux - (Already standalone)
	- Arbiter - Written in C/C++ and written straight to the ARM processor using Xilinx SDK
	- Packaged goods (All the above should be workable from an SD card)

SHA256 IP
=========

The SHA256 algorithm will be created in Vivado HLS is a High-Level Synthesis tool provided by Xilinx. The tool will then be able to output a valid IP to use within the design itself, but it does provide the HDL in case a user wants to make minor tweaks to the code (but it will be invalidated if it is regenerated).


Currently, all HLS related code is generated using the Vivado HLS 2012.4 30-day trial license provided by Xilinx.

Wiki
====
FAQ - https://github.com/stiggy87/ZynqBTC/wiki/FAQ
