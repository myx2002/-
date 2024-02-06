<?php
    session_start();
    @$userid=$_SESSION['userid'];
    include ('connect_mysqli.php');
?>

<!DOCTYPE html>
<html lang="zh">

<head>
    <meta charset="UTF-8">
    <title>图书馆 || 借书证增加</title>
    <link rel="stylesheet" href="try.css">
</head>

<body>
<div style="text-align: center;">
        <h1><strong>借书证增加</strong></h1><br>
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

    <form action="add_card.php" method="POST">
        <div style="text-align: center;">
            姓名：
            <input style="width:220px;" name="name_query" type="text" placeholder="请输入姓名">
            <br><br>
            单位：
            <input style="width:220px;" name="depart_query" type="text" placeholder="请输入单位">
            <br><br>
            类型：
            <input style="width:220px;" name="type_query" type="text" placeholder="请输入借书证类型（仅限S/T)">
            <br><br>
            <input type="submit" onmouseover="this.style.backgroundColor= '#2775b6';" onmouseout="this.style.backgroundColor='#495c69';" value="注册" name="submit">
        </div>
    </form>

        
    <?php
        error_reporting(0);
        $depart_que="";
        $name_que="";
        $type_que="";
        $final_one=" 1=1 ";
        $final_two=" 1=1 ";
        $final_three=" 1=1 ";
        if(!empty($_POST["depart_query"]))
        {
            $depart_que = $_POST["depart_query"];
            $final_three= $final_three." and department = '{$depart_que}' ";
        }
        if(!empty($_POST["name_query"]))
        {
            $name_que = $_POST["name_query"];
            $final_two= $final_two." and name = '{$name_que}' ";
        }
        if(!empty($_POST["type_query"]))
        {
            $type_que = $_POST["type_query"];
            $final_one= $final_one." and type = '{$type_que}' ";
        }

        if(!empty($_POST["submit"]))
        {
            if(!$name_que || !$type_que || !$depart_que)
            {
                echo "<script>alert('输入信息不完全，注册失败！')</script>";
                echo "<script>window.location.href='add_card.php'</script>";
            }
            else
            {
                $sql_one = "select* from card where ".$final_one." and ".$final_two." and ".$final_three.";";
                $res_one = mysqli_query($db,$sql_one);
                if( mysqli_num_rows($res_one) > 0 )
                {
                    echo "<script>alert('请勿重复注册！')</script>";
                    echo "<script>window.location.href='add_card.php'</script>";
                }
                else
                {
                    $sql_two = "insert into card(name,type,department)
                                values('{$name_que}','{$type_que}','{$depart_que}');";
                    $res_two = mysqli_query($db,$sql_two);
                    echo "<script>alert('注册成功！')</script>";
                    echo "<script>window.location.href='add_card.php'</script>";
                }
            }
        }      
    ?>
    <br><br><br>
</body>
</html>