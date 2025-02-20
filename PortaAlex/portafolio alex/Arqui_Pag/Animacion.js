// Espera a que el documento esté completamente cargado
document.addEventListener('DOMContentLoaded', function () {

    // Función para agregar la animación de desvanecimiento
    function fadeIn(element, delay = 0) {
        element.style.opacity = 0;
        element.style.transition = `opacity 1s ease-in-out ${delay}s`;
        setTimeout(() => {
            element.style.opacity = 1;
        }, delay * 1000);
    }

    // Función para agregar la animación de deslizamiento desde la izquierda
    function slideIn(element, delay = 0) {
        element.style.transform = 'translateX(-100%)';
        element.style.opacity = 0;
        element.style.transition = `transform 1s ease-out ${delay}s, opacity 1s ease-out ${delay}s`;
        setTimeout(() => {
            element.style.transform = 'translateX(0)';
            element.style.opacity = 1;
        }, delay * 1000);
    }

    // Selección de elementos para aplicar animación
    const header = document.querySelector('header');
    const navLinks = document.querySelectorAll('nav a');
    const sections = document.querySelectorAll('section');
    const footer = document.querySelector('footer');

    // Animación para el encabezado
    fadeIn(header, 0);

    // Animación para los enlaces de navegación con un retraso en secuencia
    navLinks.forEach((link, index) => {
        fadeIn(link, index * 0.2);  // Aumenta el retraso para cada enlace
    });

    // Animación para las secciones con deslizamiento desde la izquierda
    sections.forEach((section, index) => {
        slideIn(section, index * 0.3);  // Aumenta el retraso para cada sección
    });

    // Animación para el pie de página
    fadeIn(footer, 1);

});
