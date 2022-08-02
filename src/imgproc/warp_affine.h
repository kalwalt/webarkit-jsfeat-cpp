#ifndef WARP_AFFINE_H
#define WARP_AFFINE_H
#include <cstdio>
/*warp_affine: function(src, dst, transform, fill_value) {
                if (typeof fill_value === "undefined") { fill_value = 0; }
                var src_width=src.cols, src_height=src.rows, dst_width=dst.cols, dst_height=dst.rows;
                var src_d=src.data, dst_d=dst.data;
                var x=0,y=0,off=0,ixs=0,iys=0,xs=0.0,ys=0.0,a=0.0,b=0.0,p0=0.0,p1=0.0;
                var td=transform.data;
                var m00=td[0],m01=td[1],m02=td[2],
                    m10=td[3],m11=td[4],m12=td[5];

                for(var dptr = 0; y < dst_height; ++y) {
                    xs = m01 * y + m02;
                    ys = m11 * y + m12;
                    for(x = 0; x < dst_width; ++x, ++dptr, xs+=m00, ys+=m10) {
                        ixs = xs | 0; iys = ys | 0;

                        if(ixs >= 0 && iys >= 0 && ixs < (src_width - 1) && iys < (src_height - 1)) {
                            a = xs - ixs;
                            b = ys - iys;
                            off = src_width*iys + ixs;

                            p0 = src_d[off] +  a * (src_d[off+1] - src_d[off]);
                            p1 = src_d[off+src_width] + a * (src_d[off+src_width+1] - src_d[off+src_width]);

                            dst_d[dptr] = p0 + b * (p1 - p0);
                        }
                        else dst_d[dptr] = fill_value;
                    }
                }
            },
*/

void warp_affine(unsigned char* src, unsigned char* dst, unsigned char* transform, int fill_value){
    if (!fill_value) {
        printf("Empty fill_value, will set to zero\n");
        fill_value = 0;
    }
    printf("done\n");
};
#endif