<?php
    session_start();
    include 'connect_mysqli.php';
?>

<!DOCTYPE html>
<html lang="zh">

<head>
    <meta charset="UTF-8">
    <link rel="stylesheet" href="try.css">
    <title>图书管理系统 || 图书修改</title>
</head>

<body>
    <div style="text-align: center;">
        <h1><strong>图书修改</strong></h1><br>
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

    <form action="record_change.php" method="POST">
        <div style="text-align: center;">
            <input style="width:200px;" name="book_id" type="text" placeholder="请输入书籍序号:">
            <input type="submit" onmouseover="this.style.backgroundColor= '#2775b6';" onmouseout="this.style.backgroundColor='#495c69';" value="查询">
            <br><br>请输入需要修改的对映内容<br><br>
            序号&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;：
            <input style="width:220px;" name="book" type="text" placeholder="请输入序号">
            <br><br>
            类别&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;：
            <input style="width:220px;" name="category" type="text" placeholder="请输入类别">
            <br><br>
            书名&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;：
            <input style="width:220px;" name="title" type="text" placeholder="请输入书名">
            <br><br>
            出版社&nbsp;&nbsp;&nbsp;：
            <input style="width:220px;" name="press" type="text" placeholder="请输入出版社">
            <br><br>
            出版年份：
            <input style="width:220px;" name="publish_year" type="text" placeholder="请输入出版年份">
            <br><br>
            作者&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;：
            <input style="width:220px;" name="author" type="text" placeholder="请输入作者">
            <br><br>
            价格&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;：
            <input style="width:220px;" name="price" type="text" placeholder="请输入价格">
            <br><br>
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
            <th>出版社</th>
            <th>出版年份</th>
            <th>作者</th>
            <th>价格</th>
        </tr>
        <?php
            error_reporting(0);
            $result = 0;
            $book_id="";
            $book="";
            $category="";
            $title="";
            $press="";
            $publish_year="";
            $author="";
            $price="";

            $final_zero=" 1=1 ";
            $final_one=" 1=1 ";
            $final_two=" 1=1 ";
            $final_three=" 1=1 ";
            $final_four=" 1=1 ";
            $final_five=" 1=1 ";
            $final_six=" 1=1 ";
            $final_seven=" 1=1 ";

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
                    echo "<td>{$row['press']}</td>";
                    echo "<td>{$row['publish_year']}</td>";
                    echo "<td>{$row['author']}</td>";
                    echo "<td>{$row['price']}</td>";
                echo "</tr>";
            } 

            if(!empty($_POST["book"]))
            {
                $book = $_POST["book"];
                $final_zero= $final_zero." and book_id = '{$book}' ";
            }
            elseif(!empty($_POST["change"]))
            {
                echo "<script>alert('请输入需要修改记录对映的书籍序号！')</script>";
                echo "<script>window.location.href='record_change.php'</script>";
            }
            $sql_zero="select* from book where ".$final_zero.";";
            $res_zero = mysqli_query($db, $sql_zero);
            foreach ($res_zero as $row)
            {
                $category=$row['category'];
                $title=$row['title'];
                $press=$row['press'];
                $publish_year=$row['publish_year'];
                $author=$row['author'];
                $price=$row['price'];
            }

            if(!empty($_POST["category"]))
            {
                $category = $_POST["category"];
                $final_two= $final_two." and category = '{$category}' ";
                $result = 1;
            }
            if(!empty($_POST["title"]))
            {
                $title = $_POST["title"];
                $final_three= $final_three." and title = '{$title}' ";
                $result = 1;
            }
            if(!empty($_POST["press"]))
            {     
                $press = $_POST["press"];
                $final_four= $final_four." and press = '{$press}' ";
                $result = 1;
            }
            if(!empty($_POST["publish_year"]))
            {
                $publish_year = $_POST["publish_year"];
                $final_five= $final_five." and publish_year = '{$publish_year}' ";
                $result = 1;
            }
            if(!empty($_POST["author"]))
            {
                $author = $_POST["author"];
                $final_six= $final_six." and author = '{$author}' ";
                $result = 1;
            }
            if(!empty($_POST["price"]))
            {
                $price = $_POST["price"];
                $final_seven= $final_seven." and price = '{$price}' ";
                $result = 1;
            }


            if(!empty($_POST["change"]))
            {
                if($result==0)
                {
                    echo "<script>alert('空修改！')</script>";
                    echo "<script>window.location.href='record_change.php'</script>";
                }
                
                $sql_two="update book 
                          set category = '{$category}',
                              title = '{$title}',
                              press = '{$press}',
                              publish_year = '{$publish_year}',
                              author = '{$author}',
                              price = '{$price}'
                          where book_id = '{$book}';";
                $res_two = mysqli_query($db, $sql_two);
                echo "<script>alert('修改成功！')</script>";
                echo "<script>window.location.href='record_change.php'</script>";
            }
        ?>
    </table>
    <br><br><br><br>
</body>
</html>
