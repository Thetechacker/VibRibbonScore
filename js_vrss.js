class VibRibbonScore {
    #scoreCoupons;
    #coupons;
    #pascalTriangleHeight;
    #couponsValues;
    #scoreCouponsValues;

    constructor(){
        this.#scoreCoupons = "ABCDEFGHIJKLMNO";

        this.#coupons = Math.max(0, this.#scoreCoupons.length - 1);
        this.#pascalTriangleHeight = Math.max(0, ((this.#coupons * 2) - (Math.ceil(this.#coupons / 2) - Math.ceil(((this.#coupons % 2) * 1.5) + 0.5) + 2)));
        this.#couponsValues = Math.ceil(Math.max(0, this.#coupons) / 2);

        this.#scoreCouponsValues = this.generateScoreCouponsValues();

        // console.log(this.#pascalTriangleHeight, this.#coupons, this.#couponsValues);
        // console.table(Object.fromEntries(this.#scoreCouponsValues.map((vs, i) => [this.#scoreCoupons[i], vs])));
        // console.log("-".repeat(58));
    }

    generatePascalTriangle(triangleHeight){
        if(((!Number.isSafeInteger(triangleHeight) && (typeof triangleHeight !== "bigint")) || (triangleHeight < 0))) return null;

        const boolBigInt = (typeof triangleHeight === "bigint"), triangle = [ [ (boolBigInt ? 1n : 1) ] ];
    
        for(let curHeight = 0; curHeight < triangleHeight; curHeight++){
            const prevArr = triangle[triangle.length - 1], newArr = [];
        
            for(let prevArrIdx = 0; prevArrIdx < prevArr.length; prevArrIdx++){
                newArr.push((prevArr[prevArrIdx - 1] || (boolBigInt ? 0n : 0)) + prevArr[prevArrIdx]);
        
                if(prevArrIdx === (prevArr.length - 1)) newArr.push(prevArr[prevArrIdx] + (prevArr[prevArrIdx + 1] || (boolBigInt ? 0n : 0)));
            }
        
            triangle.push(newArr);
        }
    
        return triangle;
    }

    generateScoreCouponsValues(){
        let pascalTriangle = this.generatePascalTriangle(this.#pascalTriangleHeight), scoreCouponsValues = [];

        if(this.#coupons){
            scoreCouponsValues.push([...new Array(this.#couponsValues).fill(0)]);

            for(let x = 0; x < this.#coupons; x++){
                scoreCouponsValues.push([]);
            
                for(let y = 1; y < (this.#couponsValues + 1); y++){
                    scoreCouponsValues[x + 1].push(pascalTriangle[x + y][y]);
                }
            }
        }

        return scoreCouponsValues;
    }

    scoreCouponsToDecimal(scoreCouponsStr){
        if((typeof scoreCouponsStr !== "string") || (scoreCouponsStr.length !== this.#couponsValues)) return null;

        let prevScoreCouponIdx = null, res = 0;

        for(let i = 0; i < scoreCouponsStr.length; i++){
            let scoreCouponIdx = this.#scoreCoupons.split("").findIndex(coupon => coupon === scoreCouponsStr[i]);

            if((scoreCouponIdx < 0) || ((prevScoreCouponIdx !== null) && (scoreCouponIdx > prevScoreCouponIdx))) return null;

            res += this.#scoreCouponsValues[scoreCouponIdx][(scoreCouponsStr.length - 1) - i];

            prevScoreCouponIdx = scoreCouponIdx;
        }

        return res;
    }

    decimalToScoreCoupons(n){
        let findClosestScoreCouponValue = (arr, i, n) => {
            let minDiff = Infinity;
            let closestIndex = -1;
        
            for(let x = 0; x < arr.length; x++){
                const diff = (n - arr[x][i]);
                
                if((diff >= 0) && (diff < minDiff)){
                    minDiff = diff;
                    closestIndex = x;
                }
            }
        
            return closestIndex;
        }

        if(!Number.isSafeInteger(n)) return null;

        let scoreCouponsStr = "", k = Math.abs(n);

        for(let i = this.#couponsValues - 1; i >= 0; i--){
            let idxFinder = findClosestScoreCouponValue(this.#scoreCouponsValues, i, k);

            scoreCouponsStr += this.#scoreCoupons[idxFinder];
            k -= this.#scoreCouponsValues[idxFinder][i];
        }

        return scoreCouponsStr;
    }
}

const vrs = new VibRibbonScore();

console.log(vrs.decimalToScoreCoupons(727));
console.log(vrs.scoreCouponsToDecimal("FFFEEDA"));