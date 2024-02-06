<?php
    DEFINE('USER','root');
    DEFINE('PASSWORD','200224');
    DEFINE('HOST','localhost');
    DEFINE('DATABASE','library');

    $db = @mysqli_connect(HOST,USER,PASSWORD,DATABASE) OR die('Could not to connect to Mysql:'.mysqli_connect_error());
    mysqli_set_charset($db, 'utf8');
?>