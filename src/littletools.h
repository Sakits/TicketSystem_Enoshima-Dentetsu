//
// Created by JiamingShan on 2021/4/17.
//

#ifndef TRAINTICKET_LITTLETOOLS_H
#define TRAINTICKET_LITTLETOOLS_H

#define putInfoTo cout

#define check(x) {putInfoTo << #x << "=" << x << "      ";}
#define check2(x, y) {check(x)check(y)}
#define check3(x, y, z) {check2(x,y)check(z)}
#define check4(x, y, z, a) {check3(x,y,z)check(a)}
#define check5(x, y, z, a, b) {check4(x,y,z,a)check(b)}
#define check6(x, y, z, a, b, c) {check5(x,y,z,a,b)check(c)}
#define check7(x, y, z, a, b, c, d) {check6(x,y,z,a,b,c)check(d)}
#define check8(x, y, z, a, b, c, d, e) {check7(x,y,z,a,b,c,d)check(e)}
#define check9(x, y, z, a, b, c, d, e, f) {check8(x,y,z,a,b,c,d,e)check(f)}
#define check10(x, y, z, a, b, c, d, e, f, g) {check9(x,y,z,a,b,c,d,e,f)check(g)}
#define check11(x, y, z, a, b, c, d, e, f, g,h) {check10(x,y,z,a,b,c,d,e,f,g)check(h)}
#define cks(num,...) do{check##num(__VA_ARGS__) putInfoTo << endl;}while(0)
#define ck(x) do{check(x) putInfoTo << endl;} while(0)


#endif //TRAINTICKET_LITTLETOOLS_H
