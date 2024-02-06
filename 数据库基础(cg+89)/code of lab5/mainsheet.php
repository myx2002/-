<?php
    session_start();
    @$userid = $_SESSION['userid'];
    @$username = $_SESSION['username'];
    include 'connect_mysqli.php';
?>

<!DOCTYPE html>
<html lang="zh">

<head>
    <meta charset="UTF-8">
    <link rel="stylesheet" href="try.css">
    <title>图书管理系统 || 管理员主页</title>
    <style>
        body{
            background-image:url(imagetry.jpg);
            background-size:cover;
        }
    </style>
</head>

<body>
    <div style="text-align: center;">
        <h1><strong>欢迎使用图书管理系统</strong></h1><br>
        <h3 style="text-align: center">当前管理员为： <?php echo $username; ?></h3><br>
        <h3 style="text-align: center">当前管理员序号为：<?php echo $userid; ?></h3><br>
    </div>
    <nav>
        <div style="text-align: center;">
            <li class="father"><a class="kind" href="mainsheet.php">主页</a>
            <br>
            <li class="father">图书管理
            <ul class="son">
                <li><a class="kind" href="book_query.php">图书查询</a></li>
                <li><a class="kind" href="record_change.php">图书修改</a></li>
                <li><a class="kind" href="add_stock.php">更改库存</a></li>
                <li><a class="kind" href="add_single_book.php">单书入库</a></li>
                <li><a class="kind" href="add_book.php">批量入库</a></li>
            </ul>
            <li class="father">借书证管理
            <ul class="son">
                <li><a class="kind" href="card_query.php">借书证查询</a></li>
                <li><a class="kind" href="add_card.php">借书证添加</a></li>
            </ul>
            <li class="father">借还管理
            <ul class="son">
                <li><a class="kind" href="borrow_return.php">借还信息查询</a>
                <li><a class="kind" href="book_borrow.php">新的借阅</a></li>
                <li><a class="kind" href="book_return.php">归还图书</a></li>
            </ul>
            <br>
            <li class="father"><a class="kind" href="initial.php">切换管理账户</a></li>
            <br>
        </div>
        <br><br><br>
    </nav>
    <br>
</body>
</html>