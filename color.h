#ifndef color_H
#define color_H
class color {
    public:
        /** red (0 < x < 255) */
        int r;
        /** green (0 < x < 255) */
        int g;
        /** blue (0 < x < 255) */
        int b;

        color():r(0),g(0),b(0) {
        };

        /**
         * @param r: red
         * @param g: green
         * @param b: blue
         */
        color(int r, int g, int b):r(r),g(g),b(b) {
        }

        color* clamp() {
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
