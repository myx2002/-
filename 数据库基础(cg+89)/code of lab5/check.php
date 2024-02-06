<!DOCTYPE html>
<html lang="ch">

<head>
    <meta charset="UTF-8">
</head>

<body>
</body>

</html>

<?php
    error_reporting(0);
    include ('connect_mysqli.php');

    if ($_SERVER["REQUEST_METHOD"] == "POST") {
        $account = $_POST["account"];
        $password = $_POST["password"];
    }

    $mysql=" select* from admin where admin_id={$account} and password='{$password}' ";
    $dbobject= @mysqli_query($db,$mysql);

    if(@(mysqli_num_rows($dbobject)==1 )){
        session_start();
        $_SESSION['userid']=$account;
        $namesql="select admin_name from admin where admin_id={$account}";
        $result=mysqli_query($db,$namesql);
        $name=mysqli_fetch_array($result);
        $_SESSION['username']=$name['admin_name'];
        echo "<script>window.location='mainsheet.php'</script>";
    }
    else
    {
        echo "<script>alert('用户名或密码错误，请重新输入!');
                window.location='initial.php';</script>";
    }
?>