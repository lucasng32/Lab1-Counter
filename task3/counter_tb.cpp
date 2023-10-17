#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

int main(int argc, char **argv, char**env){
    int i;
    int clk;

    Verilated::commandArgs(argc, argv);
    // init top verilog instance
    Vcounter* top = new Vcounter;
    // init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace(tfp,99);
    tfp->open("counter.vcd");

    //init vbuddy
    if (vbdOpen()!=1) return(-1);
    vbdHeader("lab1: Counter");
    vbdSetMode(1);

    //initialize simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->ld = 1;
    top->v = vbdValue();

    //run simulation for many clock cycles
    for(i = 0; i<5000; i++){
        for (clk = 0; clk<2;clk++){
            tfp->dump(2*i+clk);
            top->clk = !top->clk;
            top->eval();
        }
        // vbdPlot(int(top->count), 0, 255);
        
        vbdHex(4, (int(top->count)>>16)& 0xF);
        vbdHex(3, (int(top->count)>>8)& 0xF);
        vbdHex(2, (int(top->count)>>4)& 0xF);
        vbdHex(1, int(top->count)& 0xF);
        
        vbdCycle(i+1);

        top->rst = (i<2);
        // reset to parameter value
        if (vbdFlag()){
            top->v = vbdValue();
            top->ld = 0;
        }
        else {
            top->ld = 1;
        }

        if (Verilated::gotFinish())exit(0);
    }
    vbdClose();
    tfp->close();
    exit(0);
}