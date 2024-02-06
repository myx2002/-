<?php
    session_start();
    include 'connect_mysqli.php';
?>

<!DOCTYPE html>
<html lang="zh">
<head>
    <meta charset="UTF-8">
    <title>图书管理系统 || 更改库存</title>
    <link rel="stylesheet" href="try.css">
</head>

<body>
    <div style="text-align: center;">
        <h1><strong>更改库存</strong></h1><br>
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
            <li class="father">借还管理</a>
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

    <form action="add_stock.php" method="POST">
        <div style="text-align: center;">
            <input style="width:200px;" name="book_id" type="text" placeholder="请输入书籍序号:">
            <input type="submit" onmouseover="this.style.backgroundColor= '#2775b6';" onmouseout="this.style.backgroundColor='#495c69';" value="查询">
            <br><br>
            <input style="width:130px;" name="book" type="text" placeholder="书籍序号:">
            <input style="width:130px;" name="stock" type="text" placeholder="采购/处理数量:">
            <input type="submit" onmouseover="this.style.backgroundColor= '#2775b6';" onmouseout="this.style.backgroundColor='#495c69';" value="修改" name="change">
            <br><br>
        </div>
    </form>

    <br><br>
    <table width='100%' border='1'>
        <tr>
            <th>书籍序号</th>
            <th>类别</th>
            <th>书名</th>
            <th>现有库存</th>
        </tr>
        <?php
            error_reporting(0);
            $book_id="";
            $book="";
            $stock="";
            $result=0;
            $nowstock=0;

            $final_zero=" 1=1 ";
            $final_one=" 1=1 ";

            if(!empty($_POST["book_id"]))
            {
                $book_id = $_POST["book_id"];
                $final_one= $final_one." and book_id = '{$book_id}' ";
            }
            $sql_one="select * from book where ".$final_one.";";
            $res_one = mysqli_query($db, $sql_one);
            foreach ($res_one as $row)
            {
                echo "<tr>";
                    echo "<td>{$row['book_id']}</td>";
                    echo "<td>{$row['category']}</td>";
                    echo "<td>{$row['title']}</td>";
                    echo "<td>{$row['stock']}</td>";
                echo "</tr>";
            } 

            if(!empty($_POST["book"]))
            {
                $book = $_POST["book"];
                $final_zero= $final_zero." and book_id = '{$book}' ";
            }
            elseif(!empty($_POST["change"]))
            {
                echo "<script>alert('请输入需要修改库存的书籍序号！')</script>";
                echo "<script>window.location.href='add_stock.php'</script>";
            }

            if(!empty($_POST["stock"]))
            {
                $stock = $_POST["stock"];
                $result=1;
            }
            $sql_two="select* from book where book_id='{$book}';";
            $res_two = mysqli_query($db, $sql_two);
            if(mysqli_num_rows($res_two)<1 && !empty($_POST["change"]) )
            {
                echo "<script>alert('书籍序号不存在！')</script>";
                echo "<script>window.location.href='add_stock.php'</script>";
            }
            else
            {
                foreach ($res_two as $row)
                {
                    $nowstock=$row['stock'];
                }
            }

            if(!empty($_POST["change"]))
            {
                if($result==0)
                {
                    echo "<script>alert('请输入需要改变的库存量！')</script>";
                    echo "<script>window.location.href='add_stock.php'</script>";
                }
                if($stock + $nowstock < 0)
                {
                    echo "<script>alert('库存值不能为负，修改失败！')</script>";
                    echo "<script>window.location.href='add_stock.php'</script>";
                }
                else
                {
                    $sql_three="update book set stock = stock + '{$stock}' where book_id = '{$book}';";
                    $res_two = mysqli_query($db, $sql_three);
                    echo "<script>alert('修改库存成功！')</script>";
                    echo "<script>window.location.href='add_stock.php'</script>";
                }
                
            }
        ?>
    </table>
    <br><br><br><br>
</body>
</html>
