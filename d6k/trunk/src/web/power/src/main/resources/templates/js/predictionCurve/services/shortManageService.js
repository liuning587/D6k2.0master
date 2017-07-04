/**
 * Created by reed on 2017/2/6.
 */
DaQOCloud.factory('shortCurveService',function ($http) {
    return {
        getMenuInfo:function () {
            return fetch("/menuInfo/realTimeCurveMenu",{method:'post'}).then(function(res) {
                console.log("(getMenuInfo)Response succeeded?", JSON.stringify(res.ok));
                return res.json();
            }).then(function(result){
                return result;
            })
        }
    }
})