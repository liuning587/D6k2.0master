/**
 * Created by lojoso on 2017/1/16.
 */

DaQOCloud.controller('userPwdCtrl',['$scope','$http','$stateParams','$state','loginService',function ($scope, $http, $stateParams, $state,loginService) {

    function resizeHeight() {
        var mainHeight = document.documentElement.clientHeight - 150;
        $("#left").css("height", mainHeight);
        $("#mid").css("height", mainHeight);
        $("#right").css("height", mainHeight);
        $("#huabu").css("height", mainHeight);
        $("#huabu1").css("height", mainHeight-19);
        $("#huabu2").css("height", mainHeight - 19);
    }

    $(function() {
        $('.unselected').bind("click",function() {
            var src = $('.unselected').attr("src");
            if(src == "img/unselected.png"){
                $('.unselected').attr("src","img/selected.png");
            }else{
                $('.unselected').attr("src","img/unselected.png");
            }
        });
        resizeHeight();
        $(window).resize(function() {
            resizeHeight();
        });

        var scrollTop = 0;
        var scrollLeft = 0;
        $('#huabu1').scroll(function () {

            if(scrollLeft < $(this).scrollLeft() || scrollLeft > $(this).scrollLeft()) {
                scrollLeft = $(this).scrollLeft();
                var dir = scrollLeft < $(this).scrollLeft() ? 1 : -1;
                $('#horizontal-ruler').scrollLeft(19 * dir + scrollLeft)
            }

            if(scrollTop < $(this).scrollTop() || scrollTop > $(this).scrollTop()) {
                scrollTop = $(this).scrollTop();
                var dir = scrollTop < $(this).scrollTop() ? 1 : -1;
                $('#vertical-ruler').scrollTop(19 * dir + scrollTop)

            }
        });
    });

    $(document).ready(function () {

        $('#oldPWD').bind('input', function () {//给文本框绑定input事件
            $('#prompt3').empty();

            var oldPwd = $('#oldPWD').val().trim();
            if (oldPwd === null || oldPwd === undefined || oldPwd ==='') {
                $("#newPWD").attr("disabled","disabled");
                $("#reNewPWD").attr("disabled","disabled");
                $("#modifyPwd").attr("disabled","disabled");
                return;
            }

            var reg = new RegExp("^(?![0-9]+$)(?![a-zA-Z]+$)(?!([^(0-9a-zA-Z)]|[\(\)])+$)([^(0-9a-zA-Z)]|[\(\)]|[a-zA-Z]|[0-9]){6,16}$");
            var res = reg.test(oldPwd);
            if (!res) {
                return;
            }

            $("#newPWD").removeAttr("disabled");

            $('#oldPWD').val().trim();
            if ($('#oldPWD').val().trim() != "") {
                var countryCount = 0; //国家总个数
                var notEqualCount = 0; //输入内容与国家不匹配的个数

            }
        });

        $('#newPWD').bind('input', function () {//给文本框绑定input事件
            $('#prompt3').empty();

            var oldPwd = $('#oldPWD').val().trim();
            if (oldPwd === null || oldPwd === undefined || oldPwd ==='') {
                $("#newPWD").attr("disabled","disabled");
                $("#reNewPWD").attr("disabled","disabled");
                $("#modifyPwd").attr("disabled","disabled");
                return;
            }

            var newPwd = $('#newPWD').val().trim();
            if (newPwd === null || newPwd === undefined || newPwd ==='') {
                $("#reNewPWD").attr("disabled","disabled");
                return;
            }

            var reg = new RegExp("^(?![0-9]+$)(?![a-zA-Z]+$)(?!([^(0-9a-zA-Z)]|[\(\)])+$)([^(0-9a-zA-Z)]|[\(\)]|[a-zA-Z]|[0-9]){6,16}$");
            var res = reg.test(newPwd);
            if (!res) {
                return;
            }

            $("#reNewPWD").removeAttr("disabled");

            if ($('#newPWD').val().trim() != "") {
                var countryCount = 0; //国家总个数
                var notEqualCount = 0; //输入内容与国家不匹配的个数

            }
        });

        $('#reNewPWD').bind('input', function () {//给文本框绑定input事件
            $('#prompt3').empty();

            var oldPwd = $('#oldPWD').val().trim();
            if (oldPwd === null || oldPwd === undefined || oldPwd ==='') {
                $("#newPWD").attr("disabled","disabled");
                $("#reNewPWD").attr("disabled","disabled");
                $("#modifyPwd").attr("disabled","disabled");
                return;
            }

            var reNewPwd = $('#reNewPWD').val().trim();

            var reg = new RegExp("^(?![0-9]+$)(?![a-zA-Z]+$)(?!([^(0-9a-zA-Z)]|[\(\)])+$)([^(0-9a-zA-Z)]|[\(\)]|[a-zA-Z]|[0-9]){6,16}$");
            var res = reg.test(reNewPwd);
            if (!res) {
                return;
            }

            $("#modifyPwd").removeAttr("disabled");

            if (reNewPwd != "") {
                var countryCount = 0; //国家总个数
                var notEqualCount = 0; //输入内容与国家不匹配的个数

            }
        });

    });

    $scope.modifyPwd = function(){

        var oldPwd = $('#oldPWD').val().trim();
        if (oldPwd === null || oldPwd === undefined || oldPwd ==='') {
            $('#prompt3').html("请输入旧密码");
            return;
        }

        var reg = new RegExp("^(?![0-9]+$)(?![a-zA-Z]+$)(?!([^(0-9a-zA-Z)]|[\(\)])+$)([^(0-9a-zA-Z)]|[\(\)]|[a-zA-Z]|[0-9]){6,16}$");
        var res = reg.test(oldPwd);
        if (!res) {
            $('#prompt3').html("旧密码规则有误");
            return;
        }

        var newPwd = $('#newPWD').val().trim();
        if (newPwd === null || newPwd === undefined || newPwd ==='') {
            $('#prompt3').html("请输入新密码");
            return;
        }

        var reNewPwd = $('#reNewPWD').val().trim();
        if (reNewPwd === null || reNewPwd === undefined || reNewPwd ==='') {
            $('#prompt3').html("请输入确认密码");
            return;
        }

        if ($scope.newPwd == $scope.oldPwd) {
            $('#prompt3').html("新密码不能和旧密码一样");
            return;
        }

        if ($scope.newPwd !== $scope.reNewPwd) {
            $('#prompt3').html("新密码和新密码确认不一致");
            return;
        }

        $('#prompt3').empty();

        $http({
            method: 'POST',
            params: {
                username:"admin",
                old_pwd:oldPwd,
                new_pwd:newPwd,
            },
            dataType: 'json',
            url: '/userManage/modifyPWD'
        }).success(function (data, status) {
            var strInfo = "";
            switch (data) {
                case 0:
                    strInfo = "密码修改成功";
                    $('#prompt3').html(strInfo).css("color", "green");

                    $('#oldPWD').val("");
                    $('#newPWD').val("");
                    $('#reNewPWD').val("");

                    break;
                case -1:
                    strInfo = "密码错误";
                    $('#prompt3').html(strInfo);
                    break;
                case -2:
                    strInfo = "用户名不存在";
                    $('#prompt3').html(strInfo);
                    break;
                case -3:
                    strInfo = "密码修改失败";
                    $('#prompt3').html(strInfo);
                    break;
                default:
                    $('#prompt3').html("其他错误").css("color", "green");
                    break;
            }

        }).error(function (data, status) {
            console.log("Modify password error!!!");

            $('#prompt3').html("密码修改失败");
        })

    }
}]);
