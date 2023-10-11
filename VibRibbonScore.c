/*
    VibRibbonScore

    Thetechacker

    C99
*/

#include <stdio.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define __ribbonType int32_t
#define __ribbonScoreCouponType char

const bool __isRibbonSigned = (((__ribbonType)(1) * -1) < 0);

struct __ribbonConstructor {
    __ribbonScoreCouponType *_scoreCoupons;

    __ribbonType _coupons;
    __ribbonType _couponsValues;

    __ribbonType **_scoreCouponsValues;

    __ribbonType MAX_INTEGER;
};

__ribbonType tPow(__ribbonType x, __ribbonType y){
    __ribbonType n = 1;

    for(__ribbonType i = 0; i < y; i++){
        n *= x;
    }

    return n;
}

bool __destroy__ribbonConstructor(struct __ribbonConstructor *__rc){
    if(__rc == NULL) return false;

    __rc->_scoreCoupons = NULL;

    if(__rc->_scoreCouponsValues != NULL){
        free(__rc->_scoreCouponsValues);
    }

    __rc->_coupons = 0;
    __rc->_couponsValues = 0;

    __rc->MAX_INTEGER = 0;

    return true;
}

bool __initialize__ribbonConstructor(__ribbonScoreCouponType *scoreCoupons, __ribbonType scoreCouponsSize, struct __ribbonConstructor *__rc){
    if((scoreCoupons == NULL) || (scoreCouponsSize < 0) || (__rc == NULL)) return false;

    bool status = true;

    __rc->_scoreCoupons = scoreCoupons;

    __rc->_coupons = ((1 > scoreCouponsSize) ? 0 : (scoreCouponsSize - 1));
    __rc->_couponsValues = (((__rc->_coupons) + 1) / 2);

    __rc->_scoreCouponsValues = malloc(sizeof(__ribbonType*) * (__rc->_coupons + 1));

    if(__rc->_scoreCouponsValues == NULL){
        status = false;

        goto cleanup;
    }

    __rc->_scoreCouponsValues[0] = malloc(sizeof(__ribbonType) * __rc->_couponsValues);

    if(__rc->_scoreCouponsValues[0] == NULL){
        status = false;

        goto cleanup;
    }

    for(__ribbonType i = 0; i < __rc->_couponsValues; i++){
        __rc->_scoreCouponsValues[0][i] = 0;
    }

    for(__ribbonType scoreCouponIndex = 1; scoreCouponIndex < (__rc->_coupons + 1); scoreCouponIndex++){
        __rc->_scoreCouponsValues[scoreCouponIndex] = malloc(sizeof(__ribbonType) * __rc->_couponsValues);

        if(__rc->_scoreCouponsValues[scoreCouponIndex] == NULL){
            status = false;

            goto cleanup;
        }

        __rc->_scoreCouponsValues[scoreCouponIndex][0] = scoreCouponIndex;

        for(__ribbonType valueIndex = 1; valueIndex < __rc->_couponsValues; valueIndex++){
            __rc->_scoreCouponsValues[scoreCouponIndex][valueIndex] = (__rc->_scoreCouponsValues[scoreCouponIndex][valueIndex - 1] + __rc->_scoreCouponsValues[scoreCouponIndex - 1][valueIndex]);
        }
    }

    __rc->MAX_INTEGER = 0;

    if(__rc->_coupons){
        for(__ribbonType i = 0; i < __rc->_couponsValues; i++){
            __rc->MAX_INTEGER += __rc->_scoreCouponsValues[__rc->_coupons][i];
        }
    }

    cleanup:
        if(!status) __destroy__ribbonConstructor(__rc);

    return status;
}

__ribbonType _findClosestScoreCouponIndex(struct __ribbonConstructor *__rc, __ribbonType i, __ribbonType n, bool *error){
    if(__rc == NULL){
        if(error != NULL) *error = true;

        return 0;
    }

    const __ribbonType __ribbonMax = (tPow(2, (sizeof(__ribbonType) * 8) - (__isRibbonSigned ? 1 : 0)) - 1);

    __ribbonType minDiff = __ribbonMax;
    __ribbonType closestIndex = 0;

    for(__ribbonType x = 0; x < (__rc->_coupons + 1); x++){
        __ribbonType diff = (n - __rc->_scoreCouponsValues[x][i]);

        if((diff >= 0) && (diff < minDiff)){
            minDiff = diff;
            closestIndex = x;
        }
    }

    return closestIndex;
}

__ribbonType scoreCouponsToInteger(struct __ribbonConstructor *__rc, __ribbonScoreCouponType *scoreCoupons, bool *error){
    if((__rc == NULL) || (scoreCoupons == NULL)){
        if(error != NULL) *error = true;

        return 0;
    }

    bool defPrevScoreCouponIdx = false;
    __ribbonType prevScoreCouponIdx = 0;
    __ribbonType res = 0;

    for(__ribbonType i = 0; i < __rc->_couponsValues; i++){
        bool defScoreCouponIdx = false;
        __ribbonType scoreCouponIdx = 0;

        for(__ribbonType x = 0; x < (__rc->_coupons + 1); x++){
            if(scoreCoupons[i] == __rc->_scoreCoupons[x]){
                scoreCouponIdx = x;
                defScoreCouponIdx = true;

                break;
            }
        }

        if(!defScoreCouponIdx || (defPrevScoreCouponIdx && (scoreCouponIdx > prevScoreCouponIdx))){
            if(error != NULL) *error = true;

            return 0;
        }

        res += __rc->_scoreCouponsValues[scoreCouponIdx][(__rc->_couponsValues - 1) - i];

        prevScoreCouponIdx = scoreCouponIdx;
        if(!defPrevScoreCouponIdx) defPrevScoreCouponIdx = true;
    }

    return res;
}

__ribbonScoreCouponType *integerToScoreCoupons(struct __ribbonConstructor *__rc, __ribbonType n, bool nullByte){
    if(__rc == NULL) return NULL;

    __ribbonScoreCouponType *scoreCoupons = malloc(sizeof(__ribbonScoreCouponType) * (__rc->_couponsValues + (nullByte ? 1 : 0)));

    if(scoreCoupons == NULL) return NULL;

    __ribbonType k = n;
    bool error = false;

    for(__ribbonType i = 0; i < __rc->_couponsValues; i++){
        __ribbonType revIdx = (__rc->_couponsValues) - (i + 1);
        __ribbonType idxFinder = _findClosestScoreCouponIndex(__rc, revIdx, k, &error);

        if(error){
            free(scoreCoupons);

            return NULL;
        }

        scoreCoupons[i] = __rc->_scoreCoupons[idxFinder];
        k -= __rc->_scoreCouponsValues[idxFinder][revIdx];
    }

    if(nullByte){
        scoreCoupons[__rc->_couponsValues] = 0x00;
    }

    return scoreCoupons;
}

int main(int argc, char *argv[]){
    __ribbonScoreCouponType *scoreCoupons = "ABCDEFGHIJKLMNO";
    struct __ribbonConstructor __rc;

    if(!__initialize__ribbonConstructor(scoreCoupons, strlen(scoreCoupons), &__rc)){
        fprintf(stderr, "<ERR> Something wrong happened while initializing __ribbonConstructor:__rc\n");

        return 1;
    }

    printf("%i %i %i\n", __rc._coupons, __rc._couponsValues, __rc.MAX_INTEGER);

    for(__ribbonType x = 0; x < (__rc._coupons + 1); x++){
        for(__ribbonType y = 0; y < __rc._couponsValues; y++){
            printf("__rc.scoreCouponsValues[%i][%i] = %i\n", x, y, __rc._scoreCouponsValues[x][y]);
        }
    }

    bool sctiError = false;
    int exitCode = 0;

    __ribbonScoreCouponType *itsc = integerToScoreCoupons(&__rc, 727, true);
    __ribbonType scti = scoreCouponsToInteger(&__rc, "FFFEEDA", &sctiError);

    if(itsc == NULL){
        fprintf(stderr, "<ERR> Something wrong happened while converting an integer to score coupons.\n");

        exitCode = 1;

        goto cleanup;
    }

    if(sctiError){
        fprintf(stderr, "<ERR> Something wrong happened while converting score coupons to an integer.\n");

        exitCode = 1;

        goto cleanup;
    }

    printf("%s\n", itsc);
    printf("%i\n", scti);

    cleanup:
        if(itsc != NULL) free(itsc);

        __destroy__ribbonConstructor(&__rc);

    return exitCode;
}
