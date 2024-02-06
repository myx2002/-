<?php
    session_start();
    @$userid=$_SESSION['userid'];
    include ('connect_mysqli.php');
?>

<!DOCTYPE html>
<html lang="zh">

<head>
    <meta charset="UTF-8">
    <link rel="stylesheet" href="try.css">
    <title>图书馆 || 单书入库</title>
</head>

<body>
    <div style="text-align: center;">
        <h1><strong>单本入库</strong></h1><br>
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

    <form action="add_single_book.php" method="POST">
        <div style="text-align: center;">
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
            当前库存：
            <input style="width:220px;" name="stock" type="text" placeholder="请输入当前库存">
            <br><br>
            <input type="submit" onmouseover="this.style.backgroundColor= '#2775b6';" onmouseout="this.style.backgroundColor='#495c69';" value="添加" name="submit">
            &nbsp;&nbsp;&nbsp;
            <input type="submit" onmouseover="this.style.backgroundColor= '#2775b6';" onmouseout="this.style.backgroundColor='#495c69';" value="刷新">
        </div>
    </form>

        
    <?php
        error_reporting(0);
        $category="";
        $title="";
        $press="";
        $publish_year="";
        $author="";
        $price="";
        $stock="";

        $temp=0;
        $result=0;
        
        $final_one=" 1=1 ";
        $final_two=" 1=1 ";
        $final_three=" 1=1 ";
        $final_four=" 1=1 ";
        $final_five=" 1=1 ";

        if(!empty($_POST["category"]))
        {
            $category = $_POST["category"];
            $final_one= $final_one." and category = '{$category}' ";
        }
        if(!empty($_POST["title"]))
        {
            $title = $_POST["title"];
            $final_two= $final_two." and title = '{$title}' ";
        }
        if(!empty($_POST["press"]))
        {
            $press = $_POST["press"];
            $final_three= $final_three." and press = '{$press}' ";
        }
        if(!empty($_POST["publish_year"]))
        {
            $publish_year = $_POST["publish_year"];
            $final_four= $final_four." and publish_year = '{$publish_year}' ";
        }
        if(!empty($_POST["author"]))
        {
            $author = $_POST["author"];
            $final_five= $final_five." and author = '{$author}' ";
        }
        if(!empty($_POST["price"]))
        {
            $price = $_POST["price"];
        }
        if(!empty($_POST["stock"]) || @($_POST['stock']==0))
        {
            @$stock = $_POST["stock"];
            $result=1;
        }

        if(!empty($_POST["submit"]))
        {
            if(!$category || !$title || !$press || !$publish_year || !$price || !$result || !$author)
            {
                echo "<script>alert('输入信息不完全，添加失败！')</script>";
                echo "<script>window.location.href='add_single_book.php'</script>";
            }
            else
            { 
                if($stock < 0)
                {
                    echo "<script>alert('书籍库存不能为负数！')</script>";
                    echo "<script>window.location.href='add_single_book.php'</script>";
                }
                else
                {
                    $sql_one = "select* from book where ".$final_one." and ".$final_two." and ".$final_three." and ".$final_four." 
                            and ".$final_five.";";
                    $res_one = mysqli_query($db,$sql_one);
                    if( mysqli_num_rows($res_one) > 0 )
                    {
                        echo "<script>alert('请勿重复添加！')</script>";
                        echo "<script>window.location.href='add_single_book.php'</script>";
                    }
                    else
                    {
                        $sql_two = "insert into book(category,title,press,publish_year,author,price,stock)
                                    values('{$category}','{$title}','{$press}','{$publish_year}','{$author}','{$price}','{$stock}');";
                        $res_two = mysqli_query($db,$sql_two);
                        $sql_three="select* from book where ".$final_one." and ".$final_two." and ".$final_three." and ".$final_four." 
                                    and ".$final_five.";";
                        $res_three = mysqli_query($db,$sql_three);
                        foreach ($res_three as $row)
                        {
                            $temp=$row['book_id'];
                        } 
                        ?>
                        <div style="text-align: center;">
                            <br><br><br>
                            <?php
                            echo "添加成功，该书籍对映序号为",$temp,"！";
                            ?>
                        </div>
                        <?php
                    }
                }
            }
        }      
    ?>
    <br><br><br>
</body>
</html>