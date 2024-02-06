<?php
    session_start();
    include ('connect_mysqli.php');
?>

<!DOCTYPE html>
<html lang="zh">

<head>
    <meta charset="UTF-8">
    <title>图书馆 || 借书证查询</title>
    <link rel="stylesheet" href="try.css">
</head>

<body>
    <div style="text-align: center;">
        <h1><strong>借书证查询</strong></h1><br>
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

    <form action="card_query.php" method="POST">
        <div style="text-align: center;">
            姓名查询：
            <input style="width:180px;" name="name_query" type="text" placeholder="请输入姓名">
            <input type="submit" onmouseover="this.style.backgroundColor= '#2775b6';" onmouseout="this.style.backgroundColor='#495c69';" value="查询">
            <br><br>
            卡号查询：
            <input style="width:180px;" name="card_query" type="text" placeholder="请输入借书证卡号">
            <input type="submit" onmouseover="this.style.backgroundColor= '#2775b6';" onmouseout="this.style.backgroundColor='#495c69';" value="查询">
            <br><br>
        </div>
    </form>

    <table width='100%' border= '1'>
        <tr>
            <th>序号</th>
            <th>姓名</th>
            <th>学科（系）</th>
            <th>借书证类别</th>
            <th>操作</th>
        </tr>
        <?php
            error_reporting(0);
            $card_que="";
            $name_que="";
            $final_res=" 1=1 ";
            if(!empty($_POST["card_query"]))
            {
                $card_que = $_POST["card_query"];
                $final_res= $final_res." and card_id = '{$card_que}' ";
            }
            if(!empty($_POST["name_query"]))
            {
                $name_que = $_POST["name_query"];
                $final_res= $final_res." and name = '{$name_que}' ";
            }

            $sql="select* from card where ".$final_res." order by card_id";
            $res=mysqli_query($db,$sql);
            foreach ($res as $row){
                echo "<tr>";
                echo "<td>{$row['card_id']}</td>";
                echo "<td>{$row['name']}</td>";
                echo "<td>{$row['department']}</td>";
                echo "<td>{$row['type']}</td>";
                echo "<td><a onmouseover=\"this.style.backgroundColor= '#2775b6';\" onmouseout=\"this.style.backgroundColor='#495c69';\" 
                             class=\"newtry\" style=\"margin-left:55px; margin-right:-160px;\"
                             href='card_delete.php?target={$row['card_id']}'>删除</a></td>";
                echo "</tr>";
            }
        ?>
    </table>
    <br><br><br>
</body>

</html>