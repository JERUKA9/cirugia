/*
 * Cirugía - Copyright (C) R. Danbrook 2015
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the
 *   distribution.
 * * Neither the name of the  nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <getopt.h>

#include "cirugia.h"
#include "header.h"
#include "rom.h"

extern int version;

int cir_cli_header_parse() {
	// Check the ROM header information
	
	if (cir_header_validate()) {
		
		// Check header version
		version = cir_header_get_version();
		
		if (version == 2) {
			fprintf(stdout, "Header Type: NES 2.0\n");
		}
		else {
			fprintf(stdout, "Header Type: iNES\n");
		}
		
		// Get the mapper number
		int mappernum = cir_header_get_mapper();
		fprintf(stdout, "Mapper: %d (0x%.2x)\n", mappernum, mappernum);
		
		// Get the submapper number (NES 2.0)
		if (version == 2) {
			fprintf(stdout, "Submapper: %d (0x%.2x)\n", cir_header_get_submapper(), cir_header_get_submapper());
		}
		
		// Get the PRG ROM size
		fprintf(stdout, "PRG ROM size in bytes: %d\n", cir_header_get_prgrom() * 16384);
		//fprintf(stdout, "PRG ROM size in kbytes: %d\n", cir_header_get_prgrom() * 16);
		
		// Check if PRG RAM or battery is present
		if (cir_header_get_prgram_present()) {
			fprintf(stdout, "PRG RAM size in bytes: %d\n", cir_header_get_prgram());
			//fprintf(stdout, "PRG RAM size in kbytes: %.2f\n", cir_header_get_prgram() / 1024.0);
			
			if (version == 2) {
				fprintf(stdout, "PRG NVRAM size in bytes: %d\n", cir_header_get_prgnvram());
			}
		}
		
		// Check the CHR ROM size or detect CHR RAM
		if (cir_header_get_chrrom()) {
			fprintf(stdout, "CHR ROM size in bytes: %d\n", cir_header_get_chrrom() * 8192);
			//fprintf(stdout, "CHR ROM size in kbytes: %d\n", cir_header_get_chrrom() * 8);
		}
		else {
			if (version == 2) {
				fprintf(stdout, "CHR RAM size in bytes: %d\n", cir_header_get_chrram());
				fprintf(stdout, "CHR NVRAM size in bytes: %d\n", cir_header_get_chrnvram());
			}
			else { fprintf(stdout, "CHR RAM: Present\n"); }
		}
		
		// Check the mirroring
		int mirroring = cir_header_get_mirroring();
		
		if (mirroring == 2) {
			fprintf(stdout, "Mirroring: Four screen\n");
		}
		else if (mirroring == 1) {
			fprintf(stdout, "Mirroring: Vertical\n");
		}
		else {
			fprintf(stdout, "Mirroring: Horizontal\n");
		}
		
		// Check if there's a trainer
		fprintf(stdout, "512-byte trainer: %s\n", cir_header_get_trainer() ? "Present" : "None");
		
		int system = cir_header_get_system();
		if (system == 2) { // PC-10
			fprintf(stdout, "System: PlayChoice-10\n");
		}
		else if (system == 1) { // VS. System
			fprintf(stdout, "System: VS. System\n");
			
			// Detect VS. System hardware if it's NES 2.0
			if (version == 2) {
				// Check the VS. System's PPU hardware
				int vsppu = cir_header_get_vsppu();
				
				if (vsppu == 0) {
					fprintf(stdout, "VS. System PPU: RP2C03B\n");
				}
				else if (vsppu == 1) {
					fprintf(stdout, "VS. System PPU: RP2C03G\n");
				}
				else if (vsppu == 2) {
					fprintf(stdout, "VS. System PPU: RP2C04-0001\n");
				}
				else if (vsppu == 3) {
					fprintf(stdout, "VS. System PPU: RP2C04-0002\n");
				}
				else if (vsppu == 4) {
					fprintf(stdout, "VS. System PPU: RP2C04-0003\n");
				}
				else if (vsppu == 5) {
					fprintf(stdout, "VS. System PPU: RP2C04-0004\n");
				}
				else if (vsppu == 6) {
					fprintf(stdout, "VS. System PPU: RC2C03B\n");
				}
				else if (vsppu == 7) {
					fprintf(stdout, "VS. System PPU: RC2C03C\n");
				}
				else if (vsppu == 8) {
					fprintf(stdout, "VS. System PPU: RC2C05-01\n");
				}
				else if (vsppu == 9) {
					fprintf(stdout, "VS. System PPU: RC2C05-02\n");
				}
				else if (vsppu == 10) {
					fprintf(stdout, "VS. System PPU: RC2C05-03\n");
				}
				else if (vsppu == 11) {
					fprintf(stdout, "VS. System PPU: RC2C05-04\n");
				}
				else if (vsppu == 12) {
					fprintf(stdout, "VS. System PPU: RC2C05-05\n");
				}
				
				// Check the VS. System's PPU Mode
				int vsmode = cir_header_get_vsmode();
				
				if (vsmode == 0) {
					fprintf(stdout, "VS. System Mode: Standard\n");
				}
				else if (vsmode == 1) {
					fprintf(stdout, "VS. System Mode: RBI Baseball\n");
				}
				else if (vsmode == 2) {
					fprintf(stdout, "VS. System Mode: TKO Boxing\n");
				}
				else if (vsmode == 3) {
					fprintf(stdout, "VS. System Mode: Super Xevious\n");
				}
			}
		}
		else { // Home Console
			fprintf(stdout, "System: Home Console\n");
		}
		
		// TV System
		int tvsystem = cir_header_get_tvsystem();
		if (tvsystem == 0) {
			fprintf(stdout, "TV System: NTSC\n");
		}
		else if (tvsystem == 1) {
			fprintf(stdout, "TV System: PAL\n");
		}
		else if (tvsystem == 2) {
			fprintf(stdout, "TV System: NTSC/PAL\n");
		}
		
		return 1;
	}
	else {
		fprintf(stdout, "No Header or Invalid ROM\n");
		return 0;
	}
}

void cir_cli_show_usage() {
	// Show usage
	fprintf(stdout, "Cirugía - A Surgical Tool for NES ROMs\n");
	fprintf(stdout, "Usage: cirugia [options] [FILE]\n");
	fprintf(stdout, "\nOptions:\n");
	fprintf(stdout, "  -v        Set Version (1-2)\n");
	fprintf(stdout, "  -m        Set Mapper (0-4095)\n");
	fprintf(stdout, "  -s        Set Submapper (0-15)\n");
	fprintf(stdout, "  -i        Set Mirroring (0-2)\n");
	fprintf(stdout, "  -j        Set PRG RAM Present (0-1)\n");
	fprintf(stdout, "  -t        Set Trainer (0-1)\n");
	fprintf(stdout, "  -q        Set System Type (0-2)\n");
	fprintf(stdout, "  -r        Set TV System/Region (0-2)\n");
	fprintf(stdout, "  -k        Set VS PPU Chip (0-12)\n");
	fprintf(stdout, "  -l        Set VS PPU Mode (0-3)\n");
	fprintf(stdout, "  -b        Set PRG ROM size (0-4095)\n");
	fprintf(stdout, "  -f        Set PRG RAM size (0-14)\n");
	fprintf(stdout, "  -g        Set PRG NVRAM size (0-14)\n");
	fprintf(stdout, "  -c        Set CHR ROM size (0-4095)\n");
	fprintf(stdout, "  -d        Set CHR RAM size (0-14)\n");
	fprintf(stdout, "  -e        Set CHR NVRAM size (0-14)\n");
	fprintf(stdout, "  -o        Output filename\n");
}

int main(int argc, char* argv[]) {
	// The main function
	
	if (argv[1] == NULL) {
		cir_cli_show_usage();
		exit(0);
	}
	
	// Load the ROM
	cir_rom_load(argv[argc -1]);
	
	// Split the header and the ROM
	cir_rom_split_header_rom();
	
	// Validate the ROM header
	int result = 0;
	result = cir_header_validate();
	
	if (result) {
		// Get the CRC32 checksum
		fprintf(stdout, "CRC: %X\n", cir_rom_get_crc());
		
		// Parse CLI options
		int c;
		while ((c = getopt(argc, argv, "b:c:d:e:f:g:i:j:k:l:m:o:q:r:s:t:v:")) != -1)
			switch (c) {
				case 'b': // Set the PRG ROM size (0 - 4095)
					if (atoi(optarg) >= 0 && atoi(optarg) < 4096) {
						cir_header_set_prgrom(atoi(optarg));
					}
					else {
						fprintf(stderr, "ERROR: -b must be 0 to 4096: skipping\n");
					}
					break;
				case 'c': // Set the CHR ROM size (0 - 4095)
					if (atoi(optarg) >= 0 && atoi(optarg) < 4096) {
						cir_header_set_chrrom(atoi(optarg));
					}
					else {
						fprintf(stderr, "ERROR: -c must be 0 to 4096: skipping\n");
					}
					break;
				case 'd': // Set the CHR RAM size (0 - 14)
					if (atoi(optarg) >= 0 && atoi(optarg) < 15) {
						cir_header_set_chrram(atoi(optarg));
					}
					else {
						fprintf(stderr, "ERROR: -d must be 0 to 14: skipping\n");
					}
					break;
				case 'e': // Set the CHR NVRAM size (0 - 14)
					if (atoi(optarg) >= 0 && atoi(optarg) < 15) {
						cir_header_set_chrnvram(atoi(optarg));
					}
					else {
						fprintf(stderr, "ERROR: -e must be 0 to 14: skipping\n");
					}
					break;
				case 'f': // Set the PRG RAM size (0 - 14)
					if (atoi(optarg) >= 0 && atoi(optarg) < 15) {
						cir_header_set_prgram(atoi(optarg));
					}
					else {
						fprintf(stderr, "ERROR: -f must be 0 to 14: skipping\n");
					}
					break;
				case 'g': // Set the PRG NVRAM size (0 - 14)
					if (atoi(optarg) >= 0 && atoi(optarg) < 15) {
						cir_header_set_prgnvram(atoi(optarg));
					}
					else {
						fprintf(stderr, "ERROR: -g must be 0 to 14: skipping\n");
					}
					break;
				case 'i': // Set Mirroring (0 - 2)
					if (atoi(optarg) >= 0 && atoi(optarg) < 3) {
						cir_header_set_mirroring(atoi(optarg));
					}
					else {
						fprintf(stderr, "ERROR: -i must be 0 to 2: skipping\n");
					}
					break;
				case 'j': // Set the PRG RAM present bit (0 - 1)
					if (atoi(optarg) == 0 && atoi(optarg) == 1) {
						cir_header_set_prgram_present(atoi(optarg));
					}
					else {
						fprintf(stderr, "ERROR: -j must be 0 to 1: skipping\n");
					}
					break;
				case 'k': // Set the VS PPU Chip (0 - 12)
					if (atoi(optarg) >= 0 && atoi(optarg) < 13) {
						cir_header_set_vsppu(atoi(optarg));
					}
					else {
						fprintf(stderr, "ERROR: -k must be 0 to 12: skipping\n");
					}
					break;
				case 'l': // Set the VS PPU Mode (0 - 3)
					if (atoi(optarg) >= 0 && atoi(optarg) < 4) {
						cir_header_set_vsmode(atoi(optarg));
					}
					else {
						fprintf(stderr, "ERROR: -l must be 0 to 3: skipping\n");
					}
					break;
				case 'm': // Set the mapper (0 - 4095)
					if (atoi(optarg) >= 0 && atoi(optarg) < 4096) {
						cir_header_set_mapper(atoi(optarg));
					}
					else {
						fprintf(stderr, "ERROR: -m must be 0 to 4096: skipping\n");
					}
					break;
				case 'o': // Write a new file
					if (optarg) {
						cir_rom_write(optarg);
					}
					else {
						fprintf(stderr, "ERROR: -o must specify filename\n");
					}
					break;
				case 'q': // Set System (0 - 2)
					if (atoi(optarg) >= 0 && atoi(optarg) < 3) {
						cir_header_set_system(atoi(optarg));
					}
					else {
						fprintf(stderr, "ERROR: -i must be 0 to 2: skipping\n");
					}
					break;
				case 'r': // Set TV System (0 - 2)
					if (atoi(optarg) >= 0 && atoi(optarg) < 3) {
						cir_header_set_tvsystem(atoi(optarg));
					}
					else {
						fprintf(stderr, "ERROR: -i must be 0 to 2: skipping\n");
					}
					break;
				case 's': // Set the mapper (0 - 15)
					if (atoi(optarg) >= 0 && atoi(optarg) < 16) {
						cir_header_set_submapper(atoi(optarg));
					}
					else {
						fprintf(stderr, "ERROR: -s must be 0 to 15: skipping\n");
					}
					break;
				case 't': // Set the trainer bit (0/1)
					if (atoi(optarg) == 0 || atoi(optarg) == 1) {
						cir_header_set_trainer(atoi(optarg));
					}
					else {
						fprintf(stderr, "ERROR: -t must be 0 or 1: skipping\n");
					}
					break;
				case 'v': // Set header version (1 - 2)
					if (atoi(optarg) == 1 || atoi(optarg) == 2) {
						cir_header_set_version(atoi(optarg));
					}
					else {
						fprintf(stderr, "ERROR: -v must be 1 or 2: skipping\n");
					}
					break;
				default:
					break;
			}
		
		// Spit out the new information
		cir_cli_header_parse();
	}
	
	// Exit
	cir_rom_cleanup();
	return 0;
}