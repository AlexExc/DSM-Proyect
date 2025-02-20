const express = require("express");
const bodyParser = require("body-parser");
const nodemailer = require("nodemailer");
const cors = require("cors");

const app = express();
const PORT = 3000;

// Middleware
app.use(cors());
app.use(bodyParser.json());
app.use(express.static("public"));

// Configuración de transporte para Nodemailer
const transporter = nodemailer.createTransport({
  service: "Gmail", // Puedes usar "Outlook", "Yahoo", etc.
  auth: {
    user: "tu_correo@gmail.com", // Cambia por tu correo
    pass: "tu_contraseña_de_aplicacion", // Contraseña de aplicación o contraseña SMTP
  },
});

// Endpoint para manejar el envío del correo
app.post("/send-email", (req, res) => {
  const { nombre, telefono, correo, detalles } = req.body;

  // Configuración del correo
  const mailOptions = {
    from: "tu_correo@gmail.com",
    to: correo, // Correo del cliente
    subject: "Confirmación de Cita - Barbería Estilo",
    text: `
      Hola ${nombre},
      Tu cita ha sido agendada con éxito. Aquí tienes los detalles:
      
      - Teléfono: ${telefono}
      - Detalles: ${detalles}
      
      ¡Gracias por elegir Barbería Estilo!
    `,
  };

  // Enviar el correo
  transporter.sendMail(mailOptions, (error, info) => {
    if (error) {
      console.error(error);
      return res.status(500).json({ message: "Error al enviar el correo" });
    }
    console.log("Correo enviado: " + info.response);
    res.status(200).json({ message: "Correo enviado con éxito" });
  });
});

// Iniciar el servidor
app.listen(PORT, () => {
  console.log(`Servidor corriendo en http://localhost:${PORT}`);
});
