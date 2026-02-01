pragma Singleton
import QtQuick

QtObject {
    // Background Colors
    readonly property color bgPrimary: "#0B1220"
    readonly property color bgSecondary: "#111B33"
    readonly property color bgCard: "#0E1730"
    readonly property color bgInput: "#0F172A"
    readonly property color bgInputDisabled: "#0B1220"
    readonly property color bgPopup: "#151B2A"
    readonly property color bgLog: "#0A1022"
    readonly property color bgLogHeader: "#0F1623"
    readonly property color bgDrawer: "#0A0F1C"
    readonly property color bgDrawerMid: "#0E1730"
    readonly property color bgOverlay: "#050811"
    readonly property color bgControlPanel: "#0A0F1C"

    // Border Colors
    readonly property color borderPrimary: "#1F2A4A"
    readonly property color borderSecondary: "#344666"
    readonly property color borderProgress: "#24324C"
    readonly property color borderDrawer: "#2D3B5F"

    // Accent Colors
    readonly property color accent: "#6EE7FF"
    readonly property color accent2: "#A78BFA"
    readonly property color accentError: "#F87171"
    readonly property color accentSuccess: "#22C55E"
    readonly property color accentSuccessDark: "#16A34A"
    readonly property color accentWarning: "#EF4444"
    readonly property color accentWarningDark: "#DC2626"

    // Text Colors
    readonly property color textPrimary: "#E2E8F0"
    readonly property color textSecondary: "#B8C2E0"
    readonly property color textMuted: "#94A3B8"
    readonly property color textPlaceholder: "#7C8AB0"
    readonly property color textInfo: "#93C5FD"
    readonly property color textSuccess: "#A7F3D0"
    readonly property color textError: "#FCA5A5"
    readonly property color textSuccessDark: "#052E16"
    readonly property color textErrorDark: "#7F1D1D"
    readonly property color textLog: "#C7D2FE"

    // Step Indicator
    readonly property color stepInactive: "#2A3756"
    readonly property color stepDone: "#A7F3D0"

    // Button Colors
    readonly property color buttonSecondary: "#374151"
    readonly property color buttonDisabled: "#1B2440"
    readonly property color buttonDisabledBorder: "#2A3557"

    // Shimmer/Animation
    readonly property color shimmerLight: "#BFE8FF"

    // Dialog dimensions
    readonly property int dialogWidth: 650
    readonly property int dialogHeaderHeight: 44
    readonly property int dialogRadius: 14

    // Button dimensions
    readonly property int buttonWidth: 120
    readonly property int buttonHeight: 44

    // Card dimensions
    readonly property int cardRadius: 16

    // Font sizes
    readonly property int fontSizeSmall: 10
    readonly property int fontSizeNormal: 12
    readonly property int fontSizeMedium: 13
    readonly property int fontSizeLarge: 14
    readonly property int fontSizeTitle: 16
    readonly property int fontSizeHeader: 18
    readonly property int fontSizeXLarge: 22
}
