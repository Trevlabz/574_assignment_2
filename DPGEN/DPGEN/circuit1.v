`timescale 1ns / 1ns


module circuit1(clk, rst, a, b, c, z, x);
	input clk, rst;
	input signed [7:0] a, b, c;
	
	output signed [7:0] z;
	output signed [15:0] x;
	
	wire signed [7:0] d, e;
	wire signed [15:0] f, g;
	wire signed [15:0] xwire;
	
	SADD #(.DATAWIDTH(8)) ADD0(a, b, d);
	SADD #(.DATAWIDTH(8)) ADD1(a, c, e);
	SCOMP #(.DATAWIDTH(16)) COMP2(d, e, g[0], ,);
	SMUX2x1 #(.DATAWIDTH(8)) MUX3(e, d, g, z);
	SMUL #(.DATAWIDTH(16)) MUL4(a, c, f);
	SSUB #(.DATAWIDTH(16)) SUB5(f, d, xwire);
	SREG #(.DATAWIDTH(16)) REG6(xwire, clk, rst, x);

endmodule