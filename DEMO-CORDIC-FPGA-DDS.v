
module cordic_dds
#(
   parameter DW = 16, /* Data width */
   parameter PIPE_DEPTH = 14, /* Pipeline depth */
   parameter limP = 16'h4dba /* P = 0.607253 * 2^15 */
)
(/*AUTOARG*/
   // Outputs
   sin_o, cos_o, err_o,
   // Inputs
   clk, phase_i
);

   input             clk;
   input [DW-1:0]    phase_i; /* Phase */
   output [DW:0]     sin_o, cos_o; /* Function value output */
   output [DW:0]     err_o;      /* Phase Error output */

   reg [DW:0] cos_r=0, sin_o_r=0;
   reg [DW:0] x[PIPE_DEPTH:0];
   reg [DW:0] y[PIPE_DEPTH:0];
   reg [DW:0] z[PIPE_DEPTH:0];

   reg [DW:0] atan_rom[PIPE_DEPTH:0];
   
   reg [1:0] quadrant [PIPE_DEPTH:0];

   integer i;
   initial begin
      for(i=0; i<=PIPE_DEPTH; i=i+1) begin
         x[i] = 0; y[i] = 0; z[i] = 0;
         quadrant[i] = 2'b0;
      end
   end
   
   initial begin
      atan_rom[0] <= 8189;
      atan_rom[1] <= 4834;
      atan_rom[2] <= 2554;
      atan_rom[3] <= 1296;
      atan_rom[4] <= 650;
      atan_rom[5] <= 325;
      atan_rom[6] <= 162;
      atan_rom[7] <= 81;
      atan_rom[8] <= 40;
      atan_rom[9] <= 20;
      atan_rom[10] <= 10;
      atan_rom[11] <= 5;
      atan_rom[12] <= 2;
      atan_rom[13] <= 1;
   end
   
   
   // ================= //
   // Pipeline stages   //
   // ================= //
   always @ (posedge clk) begin // stage 0
      x[0] <= {1'b0, limP};
      y[0] <= 0;
      z[0] <= {3'b0, phase_i[DW-1-2:0]}; // control the phase_i to the range[0-Pi/2]
   end

   always @ (posedge clk) begin // stage 1
      x[1] <= x[0] - y[0];
      y[1] <= x[0] + y[0];
      z[1] <= z[0] - atan_rom[0]; // reversal 45deg
   end

   generate
      genvar k;
      for(k=1; k<PIPE_DEPTH; k=k+1) begin
         always @ (posedge clk) begin
            if (z[k][DW]) begin /* the diff is negative on clockwise */
               x[k+1] <= x[k] + {{k{y[k][DW]}},y[k][DW:k]}; /* >> k */
               y[k+1] <= y[k] - {{k{x[k][DW]}},x[k][DW:k]}; /* >> k */
               z[k+1] <= z[k] + atan_rom[k];
            end else begin
               x[k+1] <= x[k] - {{k{y[k][DW]}},y[k][DW:k]};
               y[k+1] <= y[k] + {{k{x[k][DW]}},x[k][DW:k]};
               z[k+1] <= z[k] - atan_rom[k];
            end
         end
      end
   endgenerate

   // ================= //
   // Count quadrant    //
   // ================= //
   always @ (posedge clk) begin
      quadrant[0] <= phase_i[DW-1:DW-2];
   end
   generate
      genvar j;
      for(j=0; j<PIPE_DEPTH; j=j+1) begin
         always @ (posedge clk) begin
            quadrant[j+1] <= quadrant[j];
         end
      end
   endgenerate

   // ================= //
   // Adjust quadrant   //
   // ================= //
   always @ (posedge clk)
      case(quadrant[PIPE_DEPTH])
         2'b00: begin
            cos_r <= x[PIPE_DEPTH]; /* cos */
            sin_o_r <= y[PIPE_DEPTH]; /* sin */
         end
         2'b01: begin
            cos_r <= ~(y[PIPE_DEPTH]) + 1'b1; /* -sin */
            sin_o_r <= x[PIPE_DEPTH]; /* cos */
         end
         2'b10: begin
            cos_r <= ~(x[PIPE_DEPTH]) + 1'b1; /* -cos */
            sin_o_r <= ~(y[PIPE_DEPTH]) + 1'b1; /* -sin */
         end
         2'b11: begin
            cos_r <= y[PIPE_DEPTH]; /* sin */
            sin_o_r <= ~(x[PIPE_DEPTH]) + 1'b1; /* -cos */
         end
      endcase

   assign cos_o = cos_r;
   assign sin_o = sin_o_r;
   assign err_o = z[PIPE_DEPTH];

endmodule
