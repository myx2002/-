<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <title>DELETE</title>
</head>

<body>
</body>

</html>

<?php
    error_reporting(0);
    session_start();
    include 'connect_mysqli.php';

    $delete = $_GET['target'];

    $sql_two = "select* from borrow where card_id ='{$delete}' and return_time is NULL ;";
    $res_two = mysqli_query($db, $sql_two);


    if (mysqli_num_rows($res_two) >0 ) 
    {
        echo "<script>alert('不能删除该借书证，该借书证尚有外借记录！')</script>";
        echo "<script>window.location.href='card_query.php'</script>";
    } 
    else 
    {
        $sql = "delete from card where card_id ='{$delete}';";
        if(mysqli_query($db, $sql))
        {
            echo "<script>alert('删除成功！')</script>";
            echo "<script>window.location.href='card_query.php'</script>";
        }
         else
        {
            echo "<script>alert('存在未知错误，删除失败！')</script>";
            echo "<script>window.location.href='card_query.php'</script>";
        }
    }
?>