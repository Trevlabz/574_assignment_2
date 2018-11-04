`timescale 1ns / 1ns


module circuit2(clk, rst, a, b, c, z, x);
	input clk, rst;
	input signed [31:0] a, b, c;
	
	output signed [31:0] z, x;
	
	wire signed [31:0] d, e, f, g, h;
	wire signed dLTe, dEQe;
	wire signed [31:0] zwire, xwire;
	
	SADD #(.DATAWIDTH(32)) ADD0(a, b, d);
	SADD #(.DATAWIDTH(32)) ADD1(a, c, e);
	SSUB #(.DATAWIDTH(32)) SUB2(a, b, f);
	SCOMP #(.DATAWIDTH(1)) COMP3(d, e, , ,dEQe);
	SCOMP #(.DATAWIDTH(1)) COMP4(d, e, ,dLTe[0],);
	SMUX2x1 #(.DATAWIDTH(32)) MUX5(e, d, dLTe, g);
	SMUX2x1 #(.DATAWIDTH(32)) MUX6(f, g, dEQe, h);
	SSHL #(.DATAWIDTH(32)) SHL7(g, dLTe, xwire);
	SSHR #(.DATAWIDTH(32)) SHR8(h, dEQe, zwire);
	SREG #(.DATAWIDTH(32)) REG9(xwire, clk, rst, x);
	SREG #(.DATAWIDTH(32)) REG10(zwire, clk, rst, z);

endmodule