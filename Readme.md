# ESP WiFi Socket - Sistema de Comunicación Industrial Inalámbrica

## 📋 Descripción General

**ESP WiFi Socket** es un sistema de comunicación inalámbrica avanzado diseñado para **aplicaciones de control industrial**. Este proyecto implementa un puente de comunicación bidireccional entre sistemas de control y aplicaciones de monitoreo externas, proporcionando conectividad WebSocket robusta para el intercambio de datos en tiempo real.

El sistema permite el control remoto y monitoreo en tiempo real de parámetros críticos de procesos industriales, incluyendo temperaturas, tiempos, y configuraciones de equipos especializados.

## 🎯 Características Principales

### 🔌 Conectividad Avanzada
- **WebSocket Server**: Comunicación en tiempo real con múltiples clientes
- **WiFi Access Point**: Crea su propia red WiFi para conexiones dedicadas
- **Protocolo JSON**: Intercambio de datos estructurado y confiable
- **Cliente Especializado**: Integración con sistemas de monitoreo externos

### 🌡️ Monitoreo de Datos en Tiempo Real
- **Temperatura ET (Environment Temperature)**: Temperatura del ambiente de proceso
- **Temperatura BT (Bean/Batch Temperature)**: Temperatura directa del proceso
- **RoR (Rate of Rise)**: Velocidad de incremento de temperatura
- **Control de Equipos**: Porcentaje de potencia de equipos principales
- **Control de Motores**: Velocidad de rotación de componentes
- **Control de Ventilación**: Intensidad del flujo de aire

### 🛠️ Funciones de Control
- **Configuración WiFi Dinámica**: Cambio de SSID, contraseña y canal por comando
- **Heartbeat Monitoring**: Supervisión de conexión y estado del sistema
- **Eventos de Proceso**: Marcado automático de eventos críticos del proceso
- **Pairing System**: Emparejamiento seguro con dispositivos cliente
- **Persistencia EEPROM**: Almacenamiento permanente de configuraciones

### 🔧 Arquitectura Robusta
- **Gestión de Clientes Múltiples**: Soporte para conexiones simultáneas
- **Manejo de Errores**: Sistema robusto de detección y recuperación
- **Comunicación Serial**: Interfaz bidireccional con el sistema principal
- **Debug Integrado**: Sistema completo de trazas y diagnóstico

## 🏗️ Arquitectura del Sistema

```
┌─────────────────┐      ┌──────────────────┐      ┌─────────────────┐
│   Sistema       │────▶│   ESP32 WiFi      │────▶│   Aplicación    │
│   de Control    │      │   Socket         │      │   de Monitoreo  │
│   Principal     │◀────│   (Este Código)   │◀────│   Externa       │
└─────────────────┘      └──────────────────┘      └─────────────────┘
                                 │
                                 ▼
                        ┌─────────────────┐
                        │   Otros         │
                        │   Clientes      │
                        │   (Sistemas     │
                        │   Auxiliares)   │
                        └─────────────────┘
```

## 📊 Protocolo de Comunicación

### Comandos Serial (Entrada desde Sistema de Control)
```
S,<SSID>,<PASSWORD>,<CHANNEL>     # Configurar WiFi
IN,<ET>,<BT>,<EQUIP1>,<EQUIP2>,<EQUIP3>,<ROR>,<DELTA>  # Datos de proceso
MCA                               # Marcar inicio de proceso
MDR                               # Marcar fin de proceso
MFC                               # Marcar evento crítico
PAIR                             # Iniciar emparejamiento
```

### Mensajes WebSocket (JSON)
```json
{
  "deviceID": 1,
  "command": "getData",
  "data": {
    "ET": 150,
    "BT": 120,
    "RoR": 15,
    "equipo1": 75,
    "equipo2": 100,
    "equipo3": 50
  }
}
```

## 🚀 Configuración y Uso

### Hardware Requerido
- **ESP32** (cualquier variante compatible)
- **Conexión Serial** con el sistema de control principal
- **Alimentación 3.3V/5V**

### Configuración Inicial
1. **Flashear el firmware** en el ESP32
2. **Conectar serialmente** al sistema de control principal
3. **Configurar WiFi** mediante comando serial:
   ```
   S,MI_SISTEMA,MiPassword123,6
   ```
4. **Conectar aplicación cliente** a la IP: `192.168.4.1:8080`

### Integración con Aplicaciones de Monitoreo
1. Abrir aplicación de monitoreo compatible
2. Configurar conexión WebSocket:
   - **URL**: `ws://192.168.4.1:8080`
   - **Protocolo**: JSON
3. Iniciar monitoreo en tiempo real

## 📈 Casos de Uso

### 🏭 Aplicaciones Comerciales
- Monitoreo remoto de múltiples equipos
- Registro automático de perfiles de proceso
- Control de calidad en tiempo real
- Alertas de anomalías en operación

### 👨‍🔬 Investigación y Desarrollo
- Análisis detallado de curvas de proceso
- Desarrollo de nuevos perfiles
- Estudios de reproducibilidad
- Optimización de procesos industriales

### 📚 Educación y Entrenamiento
- Enseñanza de principios de control
- Demostración de efectos de parámetros
- Comparación de técnicas
- Documentación de resultados

## 🔒 Seguridad y Confiabilidad

- **Comunicación Encriptada**: Protección de datos sensibles
- **Validación de Comandos**: Verificación de integridad
- **Timeout Protection**: Prevención de bloqueos
- **Recovery Automático**: Reconexión en caso de fallas
- **Heartbeat Monitoring**: Detección de desconexiones

## 🛠️ Desarrollo y Extensibilidad

### Estructura Modular
- **Clientes Extensibles**: Fácil adición de nuevos tipos de cliente
- **Mensajes Pluggables**: Sistema de mensajería expandible
- **Adaptadores de Hardware**: Abstracción de plataforma
- **Configuración Flexible**: Parámetros ajustables en tiempo real

### APIs Disponibles
- **WebSocket API**: Para aplicaciones web
- **Serial API**: Para sistemas embebidos
- **JSON Protocol**: Para cualquier lenguaje

## 📦 Dependencias

- **ArduinoJson**: Procesamiento de mensajes JSON
- **WebSockets**: Comunicación WebSocket
- **WiFi**: Conectividad inalámbrica ESP32
- **Preferences**: Almacenamiento persistente

## 🔄 Estado del Proyecto

✅ **Funcionalidades Completadas:**
- Comunicación WebSocket bidireccional
- Integración con sistemas de monitoreo externos
- Configuración WiFi dinámica
- Monitoreo de datos en tiempo real
- Sistema de heartbeat y reconexión

🚧 **En Desarrollo:**
- Integración con sistemas auxiliares
- Interfaz web de configuración
- Logging avanzado de datos
- Certificación de seguridad

## 📞 Soporte

Este sistema está diseñado para integradores de sistemas industriales que requieren:
- Conectividad inalámbrica confiable
- Monitoreo en tiempo real
- Integración con software existente
- Flexibilidad de configuración

**Ideal para:** Fabricantes de equipos industriales, centros de investigación, sistemas de automatización, y cualquier operación que requiera control y monitoreo avanzado de procesos industriales.

---

*Desarrollado como sistema de comunicación versátil para aplicaciones de control industrial. Compatible con múltiples protocolos y estándares de la industria.*