<?php
    session_start();
    include 'connect_mysqli.php';
?>

<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <link rel="stylesheet" href="try.css">
    <title>图书管理系统 || 图书查询</title>
</head>

<body>
    <div style="text-align: center;">
        <h1><strong>图书查询</strong></h1><br>
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

    <form action="book_query.php" method="POST">
        <div style="text-align: center;">
            <br>
            类别&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;：
            <input style="width:220px;" name="category" type="text" placeholder="请输入类别">
            <br><br>
            书名&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;：
            <input style="width:220px;" name="title" type="text" placeholder="请输入书名">
            <br><br>
            出版社&nbsp;&nbsp;&nbsp;：
            <input style="width:220px;" name="press" type="text" placeholder="请输入出版社">
            <br><br>
            作者&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;：
            <input style="width:220px;" name="author" type="text" placeholder="请输入作者">
            <br><br>
            年份区间：
            <input style="width:140px;" name="publish_year_pre" type="text" placeholder="请输入起始年份">
            -
            <input style="width:140px;" name="publish_year_pos" type="text" placeholder="请输入终止年份">
            <br><br>
            价格范围：
            <input style="width:140px;" name="price_low" type="text" placeholder="请输入最低价格">
            -
            <input style="width:140px;" name="price_high" type="text" placeholder="请输入最高价格">
            <br><br>
            排序对象&nbsp;：
            <input style="width:220px;" name="order" type="text" placeholder="请输入排序对象(英语）">
            <br><br>
            <input type="submit" onmouseover="this.style.backgroundColor= '#2775b6';" onmouseout="this.style.backgroundColor='#495c69';" value="查询">
        </div>
    </form>

    <br><br>
    <table width='100%' border='1'>
        <tr>
            <th>序号</th>
            <th>类别</th>
            <th>书名</th>
            <th>出版社</th>
            <th>出版年份</th>
            <th>作者</th>
            <th>价格</th>
            <th>库存</th>
            <th>操作</th>
        </tr>
        <?php
            error_reporting(0);
            $category="";
            $title="";
            $press="";
            $publish_year_pre="";
            $publish_year_pos="";
            $author="";
            $price_low="";
            $price_high="";
            $order="book_id";

            $final_res=" 1=1 ";

            if(!empty($_POST["category"]))
            {
                $category = $_POST["category"];
                $final_res= $final_res." and category = '{$category}' ";
            }
            if(!empty($_POST["title"]))
            {
                $title = $_POST["title"];
                $final_res= $final_res." and  title like '%{$title}%' ";
            }
            if(!empty($_POST["press"]))
            {
                $press = $_POST["press"];
                $final_res= $final_res." and press like '%{$press}%' ";
            }
            if(!empty($_POST["author"]))
            {
                $author = $_POST["author"];
                $final_res= $final_res." and author like '%{$author}%' ";
            }
            if(!empty($_POST["publish_year_pre"]))
            {  
                $publish_year_pre = $_POST["publish_year_pre"];
                $final_res= $final_res." and publish_year >= {$publish_year_pre} ";
            }
            if(!empty($_POST["publish_year_pos"]))
            {  
                $publish_year_pos = $_POST["publish_year_pos"];
                $final_res= $final_res." and publish_year <= {$publish_year_pos} ";
            }
            if(!empty($_POST["price_low"]))
            {  
                $price_low = $_POST["price_low"];
                $final_res= $final_res." and price >= {$price_low} ";
            }
            if(!empty($_POST["price_high"]))
            {  
                $price_high = $_POST["price_high"];
                $final_res= $final_res." and price <= {$price_high} ";
            }
            if(!empty($_POST["order"]))
            {
                $order = $_POST["order"];
            }

            $sql="select * from book where".$final_res."order by ".$order;
            $res = mysqli_query($db, $sql);
            foreach ($res as $row)
              {
                echo "<tr>";
                    echo "<td>{$row['book_id']}</td>";
                    echo "<td>{$row['category']}</td>";
                    echo "<td>{$row['title']}</td>";
                    echo "<td>{$row['press']}</td>";
                    echo "<td>{$row['publish_year']}</td>";
                    echo "<td>{$row['author']}</td>";
                    echo "<td>{$row['price']}</td>";
                    echo "<td>{$row['stock']}</td>";
                    echo "<td><a onmouseover=\"this.style.backgroundColor= '#2775b6';\" onmouseout=\"this.style.backgroundColor='#495c69';\" 
                                 class=\"newtry\" style=\"margin-left:20px; margin-right:-50px;\"
                                 href='record_delete.php?target={$row['book_id']}'>删除</a></td>";
                echo "</tr>";
            }
        ?>
    </table>
    <br><br><br><br>
</body>
</html>


