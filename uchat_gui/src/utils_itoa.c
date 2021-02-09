#include "client.h"

// from internet
gchar *itoa(gint number) {
    static gchar buf[32] = {0};
    gint i = 30;

    if (number == 0) {
        return "0";
    }

    for (; number && i; --i, number /= 10) {
        buf[i] = "0123456789abcdef"[number % 10];
    }

    return &buf[i + 1];
}
