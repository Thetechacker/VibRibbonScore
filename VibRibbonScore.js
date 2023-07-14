/*
    VibRibbonScore

    Thetechacker
*/

class VibRibbonScore {
    #scoreCoupons;
    #coupons;
    #couponsValues;
    #pascalTriangleHeight;
    #scoreCouponsValues;
    #MAX_INTEGER;

    constructor(){
        this.#scoreCoupons = "ABCDEFGHIJKLMNO";

        this.#coupons = Math.max(0, this.#scoreCoupons.length - 1);
        this.#couponsValues = Math.max(Math.ceil(this.#coupons / 2));
        this.#pascalTriangleHeight = Math.max(0, Math.ceil((this.#coupons * 1.5) - 1));
        
        this.#scoreCouponsValues = this.generateScoreCouponsValues();

        this.#MAX_INTEGER = 0;

        if(this.#coupons){
            for(let i = 0; i < this.#scoreCouponsValues[this.#coupons].length; i++){
                this.#MAX_INTEGER += this.#scoreCouponsValues[this.#coupons][i];
            }
        }

        console.log(this.#pascalTriangleHeight, this.#coupons, this.#couponsValues, this.#MAX_INTEGER);
        console.table(Object.fromEntries(this.#scoreCouponsValues.map((vs, i) => [this.#scoreCoupons[i], vs])));
        console.log("-".repeat(58));
    }

    generatePascalTriangle(triangleHeight){
        if(!Number.isSafeInteger(triangleHeight) || (triangleHeight < 0)) return null;

        let triangle = [ [ 1 ] ];
    
        for(let curHeight = 0; curHeight < triangleHeight; curHeight++){
            let prevArr = triangle[triangle.length - 1], newArr = [];
        
            for(let prevArrIdx = 0; prevArrIdx < prevArr.length; prevArrIdx++){
                newArr.push((prevArr[prevArrIdx - 1] || 0) + prevArr[prevArrIdx]);
        
                if(prevArrIdx === (prevArr.length - 1)) newArr.push(prevArr[prevArrIdx] + (prevArr[prevArrIdx + 1] || 0));
            }
        
            triangle.push(newArr);
        }
    
        return triangle;
    }

    generateScoreCouponsValues(){
        let pascalTriangle = this.generatePascalTriangle(this.#pascalTriangleHeight), scoreCouponsValues = [];

        if(this.#coupons){
            scoreCouponsValues.push([]);

            for(let i = 0; i < this.#couponsValues; i++){
                scoreCouponsValues[0].push(0);
            }

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

        let scoreCouponSplArr = this.#scoreCoupons.split(""), prevScoreCouponIdx = null, res = 0;

        for(let i = 0; i < scoreCouponsStr.length; i++){
            let scoreCouponIdx = -1;

            for(let x = 0; x < scoreCouponSplArr.length; x++){
                if(scoreCouponSplArr[x] === scoreCouponsStr[i]){
                    scoreCouponIdx = x;

                    break;
                }
            }

            if((scoreCouponIdx < 0) || ((prevScoreCouponIdx !== null) && (scoreCouponIdx > prevScoreCouponIdx))) return null;

            res += this.#scoreCouponsValues[scoreCouponIdx][(scoreCouponsStr.length - 1) - i];

            prevScoreCouponIdx = scoreCouponIdx;
        }

        return res;
    }

    decimalToScoreCoupons(n){
        let findClosestScoreCouponIndex = (i, n) => {
            let minDiff = Infinity;
            let closestIndex = 0;
        
            for(let x = 0; x < this.#scoreCouponsValues.length; x++){
                const diff = (n - this.#scoreCouponsValues[x][i]);
                
                if((diff >= 0) && (diff < minDiff)){
                    minDiff = diff;
                    closestIndex = x;
                }
            }
        
            return closestIndex;
        };

        if(!Number.isSafeInteger(n)) return null;

        let scoreCouponsStr = "", k = n;

        for(let i = this.#couponsValues - 1; i >= 0; i--){
            let idxFinder = findClosestScoreCouponIndex(i, k);

            scoreCouponsStr += this.#scoreCoupons[idxFinder];
            k -= this.#scoreCouponsValues[idxFinder][i];
        }

        return scoreCouponsStr;
    }
}

const vrs = new VibRibbonScore();

console.log(vrs.decimalToScoreCoupons(727));
console.log(vrs.scoreCouponsToDecimal("FFFEEDA"));