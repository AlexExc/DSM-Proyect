<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $nombre = $_POST['nombre'];
    $email = $_POST['email'];
    $telefono = $_POST['telefono'];
    $asunto = $_POST['asunto'];
    $mensaje = $_POST['mensaje'];

    $to = "ar7555129@gmail.com"; 
    $subject = "Nuevo mensaje de contacto: " . $asunto;
    $body = "Nombre: $nombre\nCorreo electrónico: $email\nTeléfono: $telefono\n\nMensaje:\n$mensaje";
    $headers = "From: $email";

    if (mail($to, $subject, $body, $headers)) {
        echo "Mensaje enviado con éxito.";
    } else {
        echo "Error al enviar el mensaje.";
    }
}
?>