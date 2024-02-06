<?php
    session_start();
    //格式说明，输入数字正常，字符带''即可，用','分隔
    //不兼容已存在数据
    //绝对路径即可
    @$userid = $_SESSION['userid'];
    include 'connect_mysqli.php';
?>

<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <link rel="stylesheet" href="try.css">
    <title>图书馆 || 批量入库</title>
</head>

<body>
    <div style="text-align: center;">
        <h1><strong>批量入库</strong></h1><br>
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

    <form action="add_book.php" method="POST">
        <div style="text-align: center;">
            <input style="width:300px;" name="fileget" type="text" placeholder="请输入文件地址">            
            <br><br>
            特别注意：仅限.txt格式的绝对路径！
            (目前仅支持以逗号分隔的单独成行数据)
            <br><br>
            <input type="submit" onmouseover="this.style.backgroundColor= '#2775b6';" onmouseout="this.style.backgroundColor='#495c69';" value="添加" name="submit">
            &nbsp;&nbsp;&nbsp;
            <input type="submit" onmouseover="this.style.backgroundColor= '#2775b6';" onmouseout="this.style.backgroundColor='#495c69';" value="刷新">
        </div>
    </form>
    <br>
    <div style="text-align: center;">
        <?php
        error_reporting(0);
        if (!empty($_POST['fileget'])) 
        {
            $filename = $_POST['fileget'];
            $file = @fopen("{$filename}", "r") or exit("未找到文件!<br><br><br>");
            $flag = 1;
            $sql_zero="START TRANSACTION;";
            $res_zero = mysqli_query($db,$sql_zero);
            while (!feof($file)) 
            {
                $temp = fgets($file);
                $array = explode(',', $temp);
                $sql = "insert into book(category,title,press,publish_year,author,price,stock) 
                        values({$array[0]},{$array[1]},{$array[2]},{$array[3]},{$array[4]},{$array[5]},{$array[6]});";
                $res = mysqli_query($db, $sql);
                if (!$res) 
                {
                    $flag = 0;
                    break;
                }
            }
            if ($flag == 1) 
            {
                $sql_one="COMMIT;";
                $res_one = mysqli_query($db, $sql_one);
                fclose($file);
                echo "<script>alert('添加成功！')</script>";
                echo "<script>window.location.href='add_book.php'</script>";
            } 
            else 
            {
                $sql_one="ROLLBACK;";
                $res_one = mysqli_query($db, $sql_one);
                fclose($file);
                echo "<script>alert('文件存在错误或记录已存在，添加失败！')</script>";
                echo "<script>window.location.href='add_book.php'</script>";
            }
        }
        ?>
    </div>
</body>

</html>