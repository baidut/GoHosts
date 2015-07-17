<?php
define('PATH', 'http://hosts.52zz.gq/download/');
set_time_limit(0); // 设置允许超时
$path = file_get_contents(PATH); // 提取文件目录
preg_match('/20[0-9]{6}.txt/', $path, $matches); // 找最新的hosts文件
$hosts = file_get_contents( PATH.$matches[0]);
echo '<pre>'. $hosts .'</pre>'; // 提取hosts文件

$content = $hosts;

// 由于SAE不支持文件IO操作，写入到SAE Storage中
use sinacloud\sae\Storage as Storage;
$s = new Storage();
$domain = 'storage';
$destFileName = 'hosts.txt';
$attr = array('encoding'=>'gzip');
$storage = new SaeStorage($ak, $sk);
$result = $storage->write($domain,$destFileName, $content, -1, $attr, true);