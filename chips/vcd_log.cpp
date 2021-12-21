
#include <ctime>

#include "vcd_log.h"
#include "../circuit.h"

/* 
    VCD Log:
    
    Outputs Waveform data in the VCD file format.
*/

//TODO: Fix when multiple games try to use same file name, constructor is
// always being called even when game isn't instantiated 

VcdLogDesc::VcdLogDesc(const char* filename, 
                       int num1, const char* name1,
                       int num2, const char* name2,
                       int num3, const char* name3,
                       int num4, const char* name4,
                       int num5, const char* name5,
                       int num6, const char* name6,
                       int num7, const char* name7,
                       int num8, const char* name8,
                       int num9, const char* name9,
                       int num10, const char* name10,
                       int num11, const char* name11,
                       int num12, const char* name12,
                       int num13, const char* name13,
                       int num14, const char* name14,
                       int num15, const char* name15,
                       int num16, const char* name16,
                       int num17, const char* name17,
			           int num18, const char* name18,
			           int num19, const char* name19,
			           int num20, const char* name20,
			           int num21, const char* name21,
			           int num22, const char* name22,
			           int num23, const char* name23,
			           int num24, const char* name24,
			           int num25, const char* name25,
			           int num26, const char* name26,
			           int num27, const char* name27,
			           int num28, const char* name28,
			           int num29, const char* name29,
			           int num30, const char* name30,
			           int num31, const char* name31)
{                      
	file = fopen(filename, "w");
	fprintf(file, "$date\n");

	time_t realtime;
	time(&realtime);
	fprintf(file, "%s", ctime(&realtime));
	fprintf(file, "$end\n");

	fprintf(file, "$version\npongsim 0.01\n$end\n");
	fprintf(file, "$timescale 1ps $end\n");
	fprintf(file, "$scope module pongsim $end\n");

	char c = 'A';
	if(name1)  { fprintf(file, "$var wire 1 %c %s $end\n", c, name1);  vars[1 << (num1-1)]  = c++; }
	if(name2)  { fprintf(file, "$var wire 1 %c %s $end\n", c, name2);  vars[1 << (num2-1)]  = c++; }
	if(name3)  { fprintf(file, "$var wire 1 %c %s $end\n", c, name3);  vars[1 << (num3-1)]  = c++; }
	if(name4)  { fprintf(file, "$var wire 1 %c %s $end\n", c, name4);  vars[1 << (num4-1)]  = c++; }
	if(name5)  { fprintf(file, "$var wire 1 %c %s $end\n", c, name5);  vars[1 << (num5-1)]  = c++; }
	if(name6)  { fprintf(file, "$var wire 1 %c %s $end\n", c, name6);  vars[1 << (num6-1)]  = c++; }
	if(name7)  { fprintf(file, "$var wire 1 %c %s $end\n", c, name7);  vars[1 << (num7-1)]  = c++; }
	if(name8)  { fprintf(file, "$var wire 1 %c %s $end\n", c, name8);  vars[1 << (num8-1)]  = c++; }
	if(name9)  { fprintf(file, "$var wire 1 %c %s $end\n", c, name9);  vars[1 << (num9-1)]  = c++; }
	if(name10) { fprintf(file, "$var wire 1 %c %s $end\n", c, name10); vars[1 << (num10-1)] = c++; }
	if(name11) { fprintf(file, "$var wire 1 %c %s $end\n", c, name11); vars[1 << (num11-1)] = c++; }
	if(name12) { fprintf(file, "$var wire 1 %c %s $end\n", c, name12); vars[1 << (num12-1)] = c++; }
	if(name13) { fprintf(file, "$var wire 1 %c %s $end\n", c, name13); vars[1 << (num13-1)] = c++; }
	if(name14) { fprintf(file, "$var wire 1 %c %s $end\n", c, name14); vars[1 << (num14-1)] = c++; }
	if(name15) { fprintf(file, "$var wire 1 %c %s $end\n", c, name15); vars[1 << (num15-1)] = c++; }
    if(name16) { fprintf(file, "$var wire 1 %c %s $end\n", c, name16); vars[1 << (num16-1)] = c++; }
    if(name17) { fprintf(file, "$var wire 1 %c %s $end\n", c, name17); vars[1 << (num17-1)] = c++; }
	if(name18) { fprintf(file, "$var wire 1 %c %s $end\n", c, name18); vars[1 << (num18-1)] = c++; }
	if(name19) { fprintf(file, "$var wire 1 %c %s $end\n", c, name19); vars[1 << (num19-1)] = c++; }
	if(name20) { fprintf(file, "$var wire 1 %c %s $end\n", c, name20); vars[1 << (num20-1)] = c++; }
	if(name21) { fprintf(file, "$var wire 1 %c %s $end\n", c, name21); vars[1 << (num21-1)] = c++; }
	if(name22) { fprintf(file, "$var wire 1 %c %s $end\n", c, name22); vars[1 << (num22-1)] = c++; }
    if(name23) { fprintf(file, "$var wire 1 %c %s $end\n", c, name23); vars[1 << (num23-1)] = c++; }
    if(name24) { fprintf(file, "$var wire 1 %c %s $end\n", c, name24); vars[1 << (num24-1)] = c++; }
	if(name25) { fprintf(file, "$var wire 1 %c %s $end\n", c, name25); vars[1 << (num25-1)] = c++; }
	if(name26) { fprintf(file, "$var wire 1 %c %s $end\n", c, name26); vars[1 << (num26-1)] = c++; }
    if(name27) { fprintf(file, "$var wire 1 %c %s $end\n", c, name27); vars[1 << (num27-1)] = c++; }
    if(name28) { fprintf(file, "$var wire 1 %c %s $end\n", c, name28); vars[1 << (num28-1)] = c++; }
	if(name29) { fprintf(file, "$var wire 1 %c %s $end\n", c, name29); vars[1 << (num29-1)] = c++; }
	if(name31) { fprintf(file, "$var wire 1 %c %s $end\n", c, name31); vars[1 << (num31-1)] = c++; }
    if(name31) { fprintf(file, "$var wire 1 %c %s $end\n", c, name31); vars[1 << (num31-1)] = c++; }

	fprintf(file, "$upscope $end\n");
	fprintf(file, "$enddefinitions $end\n");
	fprintf(file, "$dumpvars\n");		
}
	
VcdLogDesc::~VcdLogDesc()
{
	fclose(file);
}

CUSTOM_LOGIC( VcdLogDesc::vcd_log )
{
	VcdLogDesc* desc = (VcdLogDesc*)chip->custom_data;
	
	chip->inputs ^= mask;

	if(desc == NULL) return;

    if(mask == 0) return;
    
    fprintf(desc->file, "#%lld\n", chip->circuit->global_time);

    for(int m = ~0; mask & m;)
    {
        int i = 1 << __builtin_ctz(mask & m);
        fprintf(desc->file, "%d%c\n", (chip->inputs & i) ? 1 : 0, desc->vars[i]);
        m &= ~i;
    }
}


CHIP_DESC( VCD_LOG ) = 
{
	CUSTOM_CHIP_START(&VcdLogDesc::vcd_log)
	    INPUT_PINS(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                   17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31),

	CHIP_DESC_END
};
