# ESP WiFi Socket - Sistema de Comunicación Industrial Inalámbrica

## 📋 Descripción General

**ESP WiFi Socket** es un sistema de comunicación inalámbrica diseñado para **aplicaciones de control industrial**. Este proyecto implementa un puente de comunicación bidireccional entre sistemas de control y aplicaciones de monitoreo externas, proporcionando conectividad WebSocket robusta para el intercambio de datos en tiempo real.

El sistema permite el control remoto y monitoreo en tiempo real de parámetros críticos de procesos industriales, incluyendo temperaturas, tiempos, y configuraciones de equipos especializados.

## 🎯 Características Principales

### 🔌 Conectividad
- **WebSocket Server**: Comunicación en tiempo real con múltiples clientes
- **WiFi Access Point**: Crea su propia red WiFi para conexiones dedicadas
- **Protocolo JSON**: Intercambio de datos estructurado y confiable
- **Cliente Especializado**: Integración con sistemas de monitoreo externos

### 🌡️ Monitoreo de Datos en Tiempo Real
- **Temperatura**: Temperatura del ambiente de proceso
- **Control de Equipos**: Porcentaje de potencia de equipos principales
- **Control de Motores**: Velocidad de rotación de componentes

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

```

### Mensajes WebSocket (JSON)
```json
{
  "deviceID": 1,
  "command": "getData",
  "data": {
    "Temperatura1": 150,
    "Temperatura2": 120,
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


### Integración con Aplicaciones de Monitoreo
1. Abrir aplicación de monitoreo compatible
2. Configurar conexión WebSocket:
   - **URL**: `ws://192.168.4.1:8080`
   - **Protocolo**: JSON
3. Iniciar monitoreo en tiempo real

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