<?php
define('PATH', 'http://hosts.52zz.gq/download/');
set_time_limit(0); // ��������ʱ
$path = file_get_contents(PATH); // ��ȡ�ļ�Ŀ¼
preg_match('/20[0-9]{6}.txt/', $path, $matches); // �����µ�hosts�ļ�
$hosts = file_get_contents( PATH.$matches[0]);
echo '<pre>'. $hosts .'</pre>'; // ��ȡhosts�ļ�

$content = $hosts;

// ����SAE��֧���ļ�IO������д�뵽SAE Storage��
use sinacloud\sae\Storage as Storage;
$s = new Storage();
$domain = 'storage';
$destFileName = 'hosts.txt';
$attr = array('encoding'=>'gzip');
$storage = new SaeStorage($ak, $sk);
$result = $storage->write($domain,$destFileName, $content, -1, $attr, true);