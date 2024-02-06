<?php
    session_start();
    include ('connect_mysqli.php');
?>

<!DOCTYPE html>
<html lang="zh">

<head>
    <meta charset="UTF-8">
    <title>图书馆 || 借还信息查询</title>
    <link rel="stylesheet" href="try.css">
</head>

<body>
    <div style="text-align: center;">
        <h1><strong>借还信息查询</strong></h1><br>
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

    <form action="borrow_return.php" method="POST">
        <div style="text-align: center;">
            卡号查询：
            <input style="width:180px;" name="card_query" type="text" placeholder="请输入借书证卡号">
            <br><br>
            姓名查询：
            <input style="width:180px;" name="name_query" type="text" placeholder="请输入姓名">
            <br><br>
            <input type="submit" onmouseover="this.style.backgroundColor= '#2775b6';" onmouseout="this.style.backgroundColor='#495c69';" value="查询">
            <br><br>
        </div>
    </form>

    <table width='100%' border= '1'>
        <tr>
            <th>借书证序号</th>
            <th>姓名</th>
            <th>部门</th>
            <th>借书证类型</th>
            <th>书籍序号</th>
            <th>书名</th>
            <th>借书时间</th>
            <th>还书时间</th>
            <th>操作人</th>
        </tr>
        <?php
            error_reporting(0);
            $card_que="";
            $name_que="";
            $final_res=" 1=1 ";
            $final_two=" 1=1 ";
            if(!empty($_POST["card_query"]))
            {
                $card_que = $_POST["card_query"];
                $final_res= $final_res." and card.card_id = '{$card_que}' ";
            }
            if(!empty($_POST["name_query"]))
            {
                $name_que = $_POST["name_query"];
                $final_two= $final_two." and name = '{$name_que}' ";
            }

            $sql="select borrow.card_id,name,department,type,book.book_id,title,borrow_time,return_time,admin_name 
                  from borrow,admin,book,card 
                  where admin.admin_id = borrow.admin_id and book.book_id = borrow.book_id and card.card_id = borrow.card_id and ".$final_res." and ".$final_two.
                  " order by borrow_time desc , book_id asc";
            $res= mysqli_query($db,$sql);
            foreach ($res as $row){
                echo "<tr>";
                echo "<td>{$row['card_id']}</td>";
                echo "<td>{$row['name']}</td>";
                echo "<td>{$row['department']}</td>";
                echo "<td>{$row['type']}</td>";
                echo "<td>{$row['book_id']}</td>";
                echo "<td>{$row['title']}</td>";
                echo "<td>{$row['borrow_time']}</td>";
                echo "<td>{$row['return_time']}</td>";
                echo "<td>{$row['admin_name']}</td>";
                echo "</tr>";
            }
        ?>
    </table>
    <br><br><br>

</body>
</html>