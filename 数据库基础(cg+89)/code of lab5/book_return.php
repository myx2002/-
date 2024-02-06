<?php
    session_start();
    include 'connect_mysqli.php';
?>

<!doctype html>
<html lang="zh">

<head>
    <meta charset="UTF-8">
    <title>图书馆 || 归还图书</title>
    <link rel="stylesheet" href="try.css">
</head>

<body>
    <div style="text-align: center;">
        <h1><strong>归还图书</strong></h1><br>
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
    
    <form action="book_return.php" method="POST">
        <div style="text-align: center;">
            <input style="width:110px;" type="text" placeholder="请输入借书证号" name="card_id">
            <input style="width:110px;" type="text" placeholder="请输入图书号" name="book_id">
            <input style="width:200px;" type="text" placeholder="请输入借阅日期(使用-连接）" name="borrow_time">
            <input style="width:200px;" type="text" placeholder="请输入还书日期(使用-连接）" name="return_time">
            <input type="submit" onmouseover="this.style.backgroundColor= '#2775b6';" onmouseout="this.style.backgroundColor='#495c69';" value="查询">
            <input type="submit" onmouseover="this.style.backgroundColor= '#2775b6';" onmouseout="this.style.backgroundColor='#495c69';" value="还书" name="submit">
            <br><br><br>
        </div>
    </form>

    <table width='100%' border = '1'>
        <?php
            error_reporting(0);
            $card_id="";
            $book_id="";
            $borrow_time="";
            $return_time="";
            $final_one=" 1=1 ";
            $final_two=" 1=1 ";
            $final_three=" 1=1 ";
            if(!empty($_POST["card_id"]))
            {
                $card_id = $_POST["card_id"];
                $final_one= $final_one." and card_id = '{$card_id}' ";
            }
            if(!empty($_POST["book_id"]))
            {
                $book_id = $_POST["book_id"];
                $final_two= $final_two." and borrow.book_id = '{$book_id}' ";
            }
            if(!empty($_POST["borrow_time"]))
            {
                $borrow_time = $_POST["borrow_time"];
                $final_three= $final_three." and borrow_time = '{$borrow_time}' ";
            }
            if(!empty($_POST["return_time"]))
            {
                $return_time = $_POST["return_time"];
            }
            $sql_zero="select card_id, borrow.book_id, title,borrow_time, return_time, stock 
                       from borrow,book
                       where borrow.book_id=book.book_id and ".$final_one." and ".$final_two." and ".$final_three." order by card_id;";
            $res_zero=mysqli_query($db,$sql_zero);
        ?>
        <tr>
            <th>借书证序号</th>
            <th>书籍序号</th>
            <th>书名</th>
            <th>借阅日期</th>
            <th>还书日期</th>
            <th>当前库存</th>
        </tr>
        <?php
            foreach ($res_zero as $row){
                echo "<tr>";
                echo "<td>{$row['card_id']}</td>";
                echo "<td>{$row['book_id']}</td>";
                echo "<td>{$row['title']}</td>";
                echo "<td>{$row['borrow_time']}</td>";
                echo "<td>{$row['return_time']}</td>";
                echo "<td>{$row['stock']}</td>";
                echo "</tr>";
            }
            $sql_one="select* from borrow where ".$final_one." and ".$final_two." and ".$final_three.";";
            $res_one=mysqli_query($db,$sql_one);

            if(!empty($_POST['submit']))
            {
                if(!$card_id || !$book_id || !$borrow_time || !$return_time)
                {
                    echo "<script>alert('请输入完整信息！')</script>";
                    echo "<script>window.location.href='book_return.php'</script>";
                }
                elseif(mysqli_num_rows($res_one) > 0 && $borrow_time <= $return_time)
                {
                    $sql_two="update borrow set return_time='{$return_time}' 
                              where book_id = '{$book_id}' and card_id = '{$card_id}' and borrow_time = '{$borrow_time}';";
                    $sql_three="update book set stock = stock + 1 
                              where book_id = '{$book_id}';";
                    $res_two=mysqli_query($db,$sql_two);
                    $res_three=mysqli_query($db,$sql_three);
                    if($res_two == 1 && $res_three == 1)
                    {
                        echo "<script>alert('还书成功！')</script>";
                        echo "<script>window.location.href='book_return.php'</script>";
                    }
                }
                else
                {
                    echo "<script>alert('还书信息输入错误，请重新输入！')</script>";
                    echo "<script>window.location.href='book_return.php'</script>";
                }
            }
        ?>
    <table>
    <br><br><br>
</body>
</html>