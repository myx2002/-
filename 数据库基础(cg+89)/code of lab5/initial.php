<?php
    session_start();
    if (isset($_SESSION['userid'])) 
    {
        unset($_SESSION['userid']);
    }
?>

<!DOCTYPE html>
<html language ="zh">

<head>
    <meta charset="UTF-8">
    <link rel="stylesheet" href="try.css">
    <title> 图书管理系统|| written by 梅岩枭</title>
    <style>
        body{
            display: flex;
            justify-content: center;
            align-items: center;
            height: 90vh;
            background-image:url(imagetry.jpg);
            background-size:cover; 
        }
        .login{
            width: 650px;
            height: 480px;
            display: flex;
            justify-content: center;
            align-items: center;
            backdrop-filter: blur(200px);
            box-shadow: 0 0 30px #5e7987;
            border-radius: 20px;
        }
        .box{
            display: flex;
            flex-direction: column;
        }
        .box input[type='text']{
            width: 400px;
            height: 100%;
            margin-bottom: 20px;
            border: 0;
            padding: 10px;
            background-color: transparent;
            border-bottom: 3px solid rgba(102,175,233,.6);
            font: 900 16px '';
        }
        .box input[type='password']{
            width: 400px;
            height: 100%;
            margin-bottom: 20px;
            border: 0;
            padding: 10px;
            background-color: transparent;
            border-bottom: 3px solid rgba(102,175,233,.6);
            font: 900 16px '';
        }
        .box input[type='submit']{
            text-align: center;
            display: block;
            height: 50px;
            width: 400px;
            padding: 12px;
            margin-left:10px; 
            font: 900 20px '';
            border-radius: 10px;
            margin-top: 20px;
            color: #fff;
            letter-spacing: 20px;
            background-image: linear-gradient(to left,
            #8abcd1,#a29bf6);
            text-decoration: none;
        }
    </style>
</head>

<body>
    <br>
    <div class="login">
        <div class="box">
            <h1>图书管理系统</h1>
            <br><br><br>
            <form action="check.php" method="POST">
                <input name="account" type="text"
                    placeholder="请输入账号" ><br><br>
                <input name="password" type="password"
                    placeholder="请输入密码"><br><br><br>
                <input type="submit" value="登陆">
                <br><br>
            </form>
        </div>
    </div>
</html>