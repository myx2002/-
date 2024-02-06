<?php
    session_start();
    @$userid = $_SESSION['userid'];
    @$username = $_SESSION['username'];
    include 'connect_mysqli.php';
?>

<!doctype html>
<html lang="zh">

<head>
    <meta charset="UTF-8">
    <title>图书馆 || 新的借阅</title>
    <link rel="stylesheet" href="try.css">
</head>

<body>
    <div style="text-align: center;">
        <h1><strong>新的借阅</strong></h1><br>
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
    
    <form action="book_borrow.php" method="POST">
        <div style="text-align: center;">
            <input type="text" placeholder="请输入借书证号" name="card_id">
            <input type="text" placeholder="请输入图书号" name="book_id">
            <input type="text" placeholder="请输入借阅日期" name="date">
            <input type="submit" onmouseover="this.style.backgroundColor= '#2775b6';" onmouseout="this.style.backgroundColor='#495c69';" value="借书" name="submit">
            <br><br><br>
        </div>
    </form>
    
    <table width='100%' border = '1'>
        <tr>
            <th>书籍序号</th>
            <th>书名</th>
            <th>类别</th>
            <th>出版社</th>
            <th>出版年份</th>
            <th>作者</th>
            <th>当前库存</th>
        </tr>
        <?php
            error_reporting(0);
            $card_id="";
            $book_id="";
            $date="";
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
                $final_two= $final_two." and book_id = '{$book_id}' ";
            }
            if(!empty($_POST["date"]))
            {
                $date = $_POST["date"];
                $final_three= $final_three." and borrow_time = '{$date}' ";
            }
            $sql_zero="select book_id, title, category, press,publish_year, author, stock from book;";
            $res_zero=mysqli_query($db,$sql_zero);
            foreach ($res_zero as $row){
                echo "<tr>";
                echo "<td>{$row['book_id']}</td>";
                echo "<td>{$row['title']}</td>";
                echo "<td>{$row['category']}</td>";
                echo "<td>{$row['press']}</td>";
                echo "<td>{$row['publish_year']}</td>";
                echo "<td>{$row['author']}</td>";
                echo "<td>{$row['stock']}</td>";
                echo "</tr>";
            }
            $sql_one="select* from card where ".$final_one ;
            $sql_two="select* from book where stock > 0 and ".$final_two ;
            $res_one=mysqli_query($db,$sql_one);
            $res_two=mysqli_query($db,$sql_two);
            if(!empty($_POST['submit']))
            {
                if(!$card_id || !$book_id || !$date)
                {
                    echo "<script>alert('输入信息不完全，无法借阅！')</script>";
                    echo "<script>window.location.href='book_borrow.php'</script>";
                }
                elseif(mysqli_num_rows($res_one) > 0)
                {
                    if(mysqli_num_rows($res_two) > 0 )
                    {
                        echo "已进入";
                        $sql_five="select* from borrow where ".$final_one." and ".$final_two." and ".$final_three.";";
                        echo $sql_five;
                        $res_five=mysqli_query($db,$sql_five);
                        if(mysqli_num_rows($res_five) < 1)
                        {
                            $sql_three="insert into borrow(card_id,book_id,borrow_time,admin_id)
                                values('{$card_id}','{$book_id}','{$date}','{$userid}');";
                            $res_three=mysqli_query($db,$sql_three);
                            $sql_four="update book set stock = stock - 1 where book_id = '{$book_id}';";
                            $res_four=mysqli_query($db,$sql_four);
                            if($res_four == 1 && $res_three == 1)
                            {
                                echo "<script>alert('借阅成功！')</script>";
                                echo "<script>window.location.href='book_borrow.php'</script>";
                            }
                        }
                        else
                        {
                            echo "<script>alert('相同借阅，请勿重复操作！')</script>";
                            echo "<script>window.location.href='book_borrow.php'</script>";
                        }
                    }
                    else
                    {
                        echo "<script>alert('书籍不存在或已借完，借书失败！')</script>";
                        echo "<script>window.location.href='book_borrow.php'</script>";
                    }
                }
                else
                {
                    echo "<script>alert('借书证不存在，借书失败！')</script>";
                    echo "<script>window.location.href='book_borrow.php'</script>";
                }
            }
        ?>
    <table>
    <br><br><br>
</body>
</html>