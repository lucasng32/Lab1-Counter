#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

int main(int argc, char **argv, char**env){
    int i;
    int clk;
    bool pause = false;
    int pausecnt = 0;

    Verilated::commandArgs(argc, argv);
    // init top verilog instance
    Vcounter* top = new Vcounter;
    // init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace(tfp,99);
    tfp->open("counter.vcd");

    //initialize simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->en = 0;

    //run simulation for many clock cycles
    for(i = 0; i<300; i++){
        for (clk = 0; clk<2;clk++){
            tfp->dump(2*i+clk);
            top->clk = !top->clk;
            top->eval();
        }

        if (pause){
            pausecnt +=1 ;
            if (pausecnt == 2){
                pause = false;
                top->en = 1;
            }
        }
        else if  (top->count == 9){
            pause  = true;
            top->en = 0;
        }
        else {
            top->rst = (i<2)|(i==10);
            top->en = i>4;
        }
        if (Verilated::gotFinish())exit(0);
    }
    tfp->close();
    exit(0);
}