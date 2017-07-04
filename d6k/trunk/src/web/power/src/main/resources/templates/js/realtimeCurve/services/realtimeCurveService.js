/**
 * Created by reed on 2017/2/6.
 */
DaQOCloud.factory('realtimeCurveService',function ($http) {
    return {
        getStationInfo:function () {
            return fetch("/GetStationInfo",{method:'post'}).then(function(res) {
                console.log("(getStationInfo)Response succeeded?", JSON.stringify(res.ok));
                return res.json();
            }).then(function(result){
                return result;
            })
        }
    }
})