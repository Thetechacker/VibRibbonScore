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
    __ribbonType _pascalTriangleHeight;

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

__ribbonType **generatePascalTriangle(__ribbonType triangleHeight){
    const __ribbonType __ribbonMax = (tPow(2, (sizeof(__ribbonType) * 8) - (__isRibbonSigned ? 1 : 0)) - 1);

    __ribbonType **triangle = malloc(sizeof(__ribbonType *) * (triangleHeight + 1));

    if(triangle == NULL) return NULL;

    bool status = true;

    triangle[0] = malloc(sizeof(__ribbonType) * 1);

    if(triangle[0] == NULL){
        status = false;

        goto cleanup;
    }

    triangle[0][0] = 1;

    for(__ribbonType curHeight = 1; curHeight < (triangleHeight + 1); curHeight++){
        triangle[curHeight] = malloc(sizeof(__ribbonType) * (curHeight + 1));

        if(triangle[curHeight] == NULL){
            for(__ribbonType freeIdx = 0; freeIdx < curHeight; freeIdx++){
                free(triangle[freeIdx]);
            }

            status = false;

            goto cleanup;
        }

        for(__ribbonType prevArrIdx = 0; prevArrIdx < curHeight; prevArrIdx++){
            triangle[curHeight][prevArrIdx] = ((__isRibbonSigned ? ((prevArrIdx - 1) < 0) : ((prevArrIdx - 1) == __ribbonMax)) ? 0 : triangle[curHeight - 1][prevArrIdx - 1]) + triangle[curHeight - 1][prevArrIdx];

            if(prevArrIdx == (curHeight - 1)) triangle[curHeight][prevArrIdx + 1] = (triangle[curHeight - 1][prevArrIdx] + (((prevArrIdx + 1) >= curHeight) ? 0 : triangle[curHeight - 1][prevArrIdx + 1]));
        }
    }

    cleanup:
        if(!status && (triangle != NULL)) free(triangle);

        return (status ? triangle : NULL);
}

__ribbonType **generateScoreCouponsValues(struct __ribbonConstructor *__rc){
    if(__rc == NULL) return NULL;

    __ribbonType **pascalTriangle = generatePascalTriangle(__rc->_pascalTriangleHeight);

    if(pascalTriangle == NULL) return NULL;

    __ribbonType **scoreCouponsValues = malloc(sizeof(__ribbonType*) * (__rc->_coupons + 1));

    if(scoreCouponsValues == NULL) return NULL;

    bool status = true;

    if(__rc->_coupons){
        scoreCouponsValues[0] = malloc(sizeof(__ribbonType) * __rc->_couponsValues);

        if(scoreCouponsValues[0] == NULL){
            status = false;

            goto cleanup;
        }

        for(__ribbonType i = 0; i < __rc->_couponsValues; i++){
            scoreCouponsValues[0][i] = 0;
        }

        for(__ribbonType x = 1; x < (__rc->_coupons + 1); x++){
            scoreCouponsValues[x] = malloc(sizeof(__ribbonType) * __rc->_couponsValues);

            if(scoreCouponsValues[x] == NULL){
                for(__ribbonType freeIdx = 0; freeIdx < (x + 1); freeIdx++){
                    free(scoreCouponsValues[freeIdx]);
                }

                status = false;

                goto cleanup;
            }

            for(__ribbonType y = 1; y < (__rc->_couponsValues + 1); y++){
                scoreCouponsValues[x][y - 1] = pascalTriangle[(x - 1) + y][y];
            }
        }
    }

    cleanup:
        if(!status && (scoreCouponsValues != NULL)) free(scoreCouponsValues);

        return (status ? scoreCouponsValues : NULL);
}

bool __initialize__ribbonConstructor(__ribbonScoreCouponType *scoreCoupons, __ribbonType scoreCouponsSize, struct __ribbonConstructor *__rc){
    if((scoreCoupons == NULL) || (scoreCouponsSize < 0) || (__rc == NULL)) return false;

    bool status = true;

    __rc->_scoreCoupons = scoreCoupons;

    __rc->_coupons = ((1 > scoreCouponsSize) ? 0 : (scoreCouponsSize - 1));
    __rc->_couponsValues = (((__rc->_coupons) + 1) / 2);
    __rc->_pascalTriangleHeight = ((1 > __rc->_coupons) ? 0 : (((__rc->_coupons) * 3) / 2 - 1 + (((__rc->_coupons) * 3) % 2)));

    __rc->_scoreCouponsValues = generateScoreCouponsValues(__rc);

    if(__rc->_scoreCouponsValues == NULL){
        status = false;

        goto cleanup;
    }

    __rc->MAX_INTEGER = 0;

    if(__rc->_coupons){
        for(__ribbonType i = 0; i < __rc->_couponsValues; i++){
            __rc->MAX_INTEGER += __rc->_scoreCouponsValues[__rc->_coupons][i];
        }
    }

    cleanup:
        if(!status){
            __rc->_scoreCoupons = NULL;

            __rc->_coupons = 0;
            __rc->_couponsValues = 0;
            __rc->_pascalTriangleHeight = 0;

            if(__rc->_scoreCouponsValues != NULL){
                free(__rc->_scoreCouponsValues);

                __rc->_scoreCouponsValues = NULL;
            }

            __rc->MAX_INTEGER = 0;
        }

        return status;
}

bool __destroy__ribbonConstructor(struct __ribbonConstructor *__rc){
    if(__rc == NULL) return false;

    __rc->_scoreCoupons = NULL;

    if(__rc->_scoreCouponsValues != NULL){
        if(__rc->_coupons){
            for(__ribbonType freeIdx = 0; freeIdx < (__rc->_coupons + 1); freeIdx++){
                free(__rc->_scoreCouponsValues[freeIdx]);
            }
        }

        free(__rc->_scoreCouponsValues);

        __rc->_scoreCouponsValues = NULL;
    }

    __rc->_coupons = 0;
    __rc->_couponsValues = 0;
    __rc->_pascalTriangleHeight = 0;

    __rc->MAX_INTEGER = 0;

    return true;
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
        printf("<ERR> Something wrong happened while initializing __ribbonConstructor:__rc\n");

        return 1;
    }

    printf("%i %i %i %i\n", __rc._pascalTriangleHeight, __rc._coupons, __rc._couponsValues, __rc.MAX_INTEGER);

    for(__ribbonType x = 0; x < (__rc._coupons + 1); x++){
        for(__ribbonType y = 0; y < __rc._couponsValues; y++){
            printf("__rc.scoreCouponsValues[%i][%i] = %i\n", x, y, __rc._scoreCouponsValues[x][y]);
        }
    }

    bool sctiError = false;

    __ribbonScoreCouponType *itsc = integerToScoreCoupons(&__rc, 727, true);
    __ribbonType scti = scoreCouponsToInteger(&__rc, "FFFEEDA", &sctiError);

    if(itsc == NULL){
        printf("<ERR> Something wrong happened when converting an integer to score coupons.\n");

        return 1;
    }

    if(sctiError){
        printf("<ERR> Something wrong happened when converting score coupons to an integer.\n");

        return 1;
    }

    printf("%s\n", itsc);
    printf("%i\n", scti);

    free(itsc);

    __destroy__ribbonConstructor(&__rc);

    return 0;
}
