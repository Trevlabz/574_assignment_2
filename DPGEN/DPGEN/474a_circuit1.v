
input signed [7:0] a, b, c;

output signed [7:0] z;
output signed [15:0]  x;

wire signed [7:0] d, e;
wire signed [15:0]  f, g;
wire signed [15:0]  xwire;

d = a + b
e = a + c
g = d > e
z = g ? d : e
f = a * c
xwire = f - d  
x = xwire
