/**
 * Created by reed on 2016/11/23.
 */

DaQOCloud.controller('loginCtrl',['$scope','$stateParams','$state','loginService',function ($scope, $stateParams, $state,loginService) {

    $(function () {
        $('body').removeClass();
        $('body').addClass('body');

    })

    $scope.login = function() {
        var userName = $('#userIdInput').val(),
            passWord = $('#passwordInput').val();

        if (userName === null || userName === undefined || userName ==='' ||
            passWord === null || passWord === undefined || passWord ==='') {
            alert('用户名和密码不能为空！');
            return;
        }

        loginService.loginMessage(userName,passWord).then(function (result) {
            if(result.nStatus == 0){
                $scope.logininfo.userInfo =result.userInfo;
                $state.go('home');
            }else if(result.nStatus == 1){
                alert('登录失败，用户名错误！');
            }else{
                alert('登录失败，密码错误！');
            }
        });
    }

    $scope.rememberPassWord = function () {
        var src = $('.unselected').attr("src");
        if(src == "img/unselected.png"){
            $('.unselected').attr("src","img/selected.png");
        }else{
            $('.unselected').attr("src","img/unselected.png");
        }
    }

}]);
