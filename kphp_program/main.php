<?php
print("<strong>This webpage is served by " . PHP_SAPI . "</strong><br>");
print("It is currently " . date("H:i:s", time() + 7200) . " in Omsk<br>");
$mc = new Memcache();
$mc->connect("127.0.0.1", "11211");
echo("Random 8 hex bytes from random-engine: " . $mc->get("hex_random16"));
?>
