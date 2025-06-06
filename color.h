#ifndef color_H
#define color_H
class color {
    public:
        int r;
        int g;
        int b;

        color():r(0),g(0),b(0) {
        };

        color(int r, int g, int b):g(g),r(r),b(b) {
        }

        color* normalize() {
            r = (r < 0)? 0: r;
            g = (g < 0)? 0: g;
            b = (b < 0)? 0: b;

            r = (r > 255)? 255: r;
            g = (g > 255)? 255: g;
            b = (b > 255)? 255: b;
            return this;
        }
};
#endif
