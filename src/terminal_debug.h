#ifndef __TERMINAL_DEBUG
#define __TERMINAL_DEBUG

/*
  Debug.h - Singleton multicaster for TerminalLibrary::OutputInterface sinks
  - Header-only
  - Thread-safe (Arduino-friendly locking)
  - Non-owning sink pointers
  - Compiles out to no-ops when TERMINAL_ENABLE_DEBUG is not defined
*/

#include <Terminal.h> // OutputInterface, COLOR, PRINT_TYPES (optional), Arduino String
#include <algorithm>
#include <cstdarg>
#include <cstdio>
#include <vector>

namespace TerminalLibrary {

// ============================================================================
// Build-time switch
// ============================================================================

/**
 * If TERMINAL_ENABLE_DEBUG is not defined, this header provides a stubbed
 * singleton whose methods are no-ops. Call sites compile, and the optimizer
 * removes all code and strings.
 */
#if !defined(TERMINAL_ENABLE_DEBUG)

// --------------------------- NO-OP (Release) --------------------------------
class Debug {
public:
  static Debug& instance() {
    static Debug inst; // still a trivial singleton for call-site compatibility
    return inst;
  }

  inline void registerOutput(OutputInterface*) {}
  inline void deregisterOutput(OutputInterface*) {}
  inline void print(const String&) {}
  inline void print(COLOR, const String&) {}
  inline void println() {}
  inline void println(const String&) {}
  inline void printf(const char*, ...) {}
  inline void printf(COLOR, const char*, ...) {}

#ifdef TERMINAL_LOGGING
  inline void print(PRINT_TYPES, const String&) {}
  inline void print(PRINT_TYPES, const String&, const String&) {}
  inline void println(PRINT_TYPES, const String&) {}
  inline void println(PRINT_TYPES, const String&, const String&) {}
#endif

#ifdef TERMINAL_HEX_STRING
  inline void hexdump(unsigned char*, unsigned long) {}
#endif

#ifdef TERMINAL_BANNER
  inline void banner() {}
#endif

  inline void prompt() {}

private:
  Debug() = default;
  Debug(const Debug&) = delete;
  Debug& operator=(const Debug&) = delete;
};

#else
// --------------------------- ACTIVE (Debug) ----------------------------------

/**
 * Locking:
 *  - Default: disable interrupts during tiny critical sections (Arduino-safe)
 *  - Optional: define TERMINAL_USE_SPINLOCK to use std::atomic_flag spinlock
 */
#if defined(TERMINAL_USE_SPINLOCK)
#include <atomic>
#endif

class Debug {
public:
  // --- Singleton access ---
  static Debug& instance() {
    // Since we only protect tiny registry changes and do fan-out lock-free,
    // function-static init is fine (C++11 thread-safe initialization).
    static Debug inst;
    return inst;
  }

  Debug(const Debug&) = delete;
  Debug& operator=(const Debug&) = delete;

  // Register a sink (no ownership). Duplicate-safe.
  inline void registerOutput(OutputInterface* out) {
    if (!out) return;
    LockGuard g(*this);
    if (std::find(sinks_.begin(), sinks_.end(), out) == sinks_.end()) { sinks_.push_back(out); }
  }

  // Deregister a sink. Safe to call even if not present.
  inline void deregisterOutput(OutputInterface* out) {
    if (!out) return;
    LockGuard g(*this);
    auto it = std::remove(sinks_.begin(), sinks_.end(), out);
    sinks_.erase(it, sinks_.end());
  }

  // ---------------------------------------------------------------------------
  // Plain printing
  // ---------------------------------------------------------------------------

  inline void print(const String& line) {
    auto snapshot = snapshotSinks_();
    for (auto* w : snapshot) {
      if (w) w->print(TRACE, line);
    }
  }

  // Print with color to all sinks
  inline void print(COLOR color, const String& line) {
    auto snapshot = snapshotSinks_();
    for (auto* w : snapshot) {
      if (w) w->print(color, line);
    }
  }

  // println() blank line
  inline void println() {
    auto snapshot = snapshotSinks_();
    for (auto* w : snapshot) {
      if (w) w->println();
    }
  }

  // println(line) — supports both TERMINAL_LOGGING (no println(String)) and no-logging
  inline void println(const String& line) {
    auto snapshot = snapshotSinks_();
    for (auto* w : snapshot) {
      if (!w) continue;
#ifdef TERMINAL_LOGGING
      w->print(TRACE, line);
      w->println();
#else
      w->println(line);
#endif
    }
  }

  // ---------------------------------------------------------------------------
  // printf-style helpers
  // ---------------------------------------------------------------------------

  inline void printf(const char* fmt, ...) {
    if (!fmt) return;

    char small[128];
    va_list args;
    va_start(args, fmt);

    va_list copy;
    va_copy(copy, args);
    int n = std::vsnprintf(small, sizeof(small), fmt, copy);
    va_end(copy);

    if (n >= 0 && (size_t) n < sizeof(small)) {
      String s(small);
      va_end(args);
      print(s);
      return;
    }

    int needed = (n < 0) ? 256 : (n + 1);
    std::vector<char> buf((size_t) needed, '\0');
    std::vsnprintf(buf.data(), buf.size(), fmt, args);
    va_end(args);

    String s(buf.data());
    print(s);
  }

  inline void printf(COLOR color, const char* fmt, ...) {
    if (!fmt) return;

    char small[128];
    va_list args;
    va_start(args, fmt);

    va_list copy;
    va_copy(copy, args);
    int n = std::vsnprintf(small, sizeof(small), fmt, copy);
    va_end(copy);

    if (n >= 0 && (size_t) n < sizeof(small)) {
      String s(small);
      va_end(args);
      print(color, s);
      return;
    }

    int needed = (n < 0) ? 256 : (n + 1);
    std::vector<char> buf((size_t) needed, '\0');
    std::vsnprintf(buf.data(), buf.size(), fmt, args);
    va_end(args);

    String s(buf.data());
    print(color, s);
  }

  // ---------------------------------------------------------------------------
  // Optional pass-throughs per your interface toggles
  // ---------------------------------------------------------------------------

#ifdef TERMINAL_LOGGING
  inline void print(PRINT_TYPES type, const String& line) {
    auto snapshot = snapshotSinks_();
    for (auto* w : snapshot) {
      if (w) w->print(type, line);
    }
  }

  inline void print(PRINT_TYPES type, const String& line, const String& line2) {
    auto snapshot = snapshotSinks_();
    for (auto* w : snapshot) {
      if (w) w->print(type, line, line2);
    }
  }

  inline void println(PRINT_TYPES type, const String& line) {
    auto snapshot = snapshotSinks_();
    for (auto* w : snapshot) {
      if (w) w->println(type, line);
    }
  }

  inline void println(PRINT_TYPES type, const String& line, const String& line2) {
    auto snapshot = snapshotSinks_();
    for (auto* w : snapshot) {
      if (w) w->println(type, line, line2);
    }
  }
#endif // TERMINAL_LOGGING

#ifdef TERMINAL_HEX_STRING
  inline void hexdump(unsigned char* buffer, unsigned long length) {
    auto snapshot = snapshotSinks_();
    for (auto* w : snapshot) {
      if (w) w->hexdump(buffer, length);
    }
  }
#endif // TERMINAL_HEX_STRING

#ifdef TERMINAL_BANNER
  inline void banner() {
    auto snapshot = snapshotSinks_();
    for (auto* w : snapshot) {
      if (w) w->banner();
    }
  }
#endif // TERMINAL_BANNER

  inline void prompt() {
    auto snapshot = snapshotSinks_();
    for (auto* w : snapshot) {
      if (w) w->prompt();
    }
  }

private:
  Debug() = default;

  // ---- Locking primitives ---------------------------------------------------

#if defined(TERMINAL_USE_SPINLOCK)
  std::atomic_flag spin_ = ATOMIC_FLAG_INIT;

  struct LockGuard {
    Debug& d_;
    explicit LockGuard(Debug& d) : d_(d) {
      while (d_.spin_.test_and_set(std::memory_order_acquire)) {
        // busy wait (critical sections are tiny)
      }
    }
    ~LockGuard() { d_.spin_.clear(std::memory_order_release); }
  };
#else
  // Default: disable interrupts briefly for tiny critical sections
  struct LockGuard {
    explicit LockGuard(Debug&) { noInterrupts(); }
    ~LockGuard() { interrupts(); }
  };
#endif

  // Copy registry under lock so we can call sinks without holding the lock
  inline std::vector<OutputInterface*> snapshotSinks_() {
    LockGuard g(*this);
    return sinks_;
  }

private:
  // Non-owning pointers; ensure deregister before sink destruction.
  std::vector<OutputInterface*> sinks_;
};

#endif // TERMINAL_ENABLE_DEBUG

// ============================================================================
// Convenience macros (zero-cost in Release)
// ============================================================================

#if defined(TERMINAL_ENABLE_DEBUG)
#define DBG_PRINT(s)                                                                                                   \
  do { ::TerminalLibrary::Debug::instance().print((s)); } while (0)
#define DBG_PRINTC(c, s)                                                                                               \
  do { ::TerminalLibrary::Debug::instance().print((c), (s)); } while (0)
#define DBG_PRINTLN()                                                                                                  \
  do { ::TerminalLibrary::Debug::instance().println(); } while (0)
#define DBG_PRINTLNS(s)                                                                                                \
  do { ::TerminalLibrary::Debug::instance().println((s)); } while (0)
#define DBG_PRINTF(fmt, ...)                                                                                           \
  do { ::TerminalLibrary::Debug::instance().printf((fmt), ##__VA_ARGS__); } while (0)
#define DBG_PRINTFC(c, fmt, ...)                                                                                       \
  do { ::TerminalLibrary::Debug::instance().printf((c), (fmt), ##__VA_ARGS__); } while (0)
#define DBG_REGISTER(o)                                                                                                \
  do { ::TerminalLibrary::Debug::instance().registerOutput((o)); } while (0)
#define DBG_DEREGISTER(o)                                                                                              \
  do { ::TerminalLibrary::Debug::instance().deregisterOutput((o)); } while (0)
#else
#define DBG_PRINT(s)                                                                                                   \
  do {                                                                                                                 \
  } while (0)
#define DBG_PRINTC(c, s)                                                                                               \
  do {                                                                                                                 \
  } while (0)
#define DBG_PRINTLN()                                                                                                  \
  do {                                                                                                                 \
  } while (0)
#define DBG_PRINTLNS(s)                                                                                                \
  do {                                                                                                                 \
  } while (0)
#define DBG_PRINTF(fmt, ...)                                                                                           \
  do {                                                                                                                 \
  } while (0)
#define DBG_PRINTFC(c, fmt, ...)                                                                                       \
  do {                                                                                                                 \
  } while (0)
#define DBG_REGISTER(o)                                                                                                \
  do {                                                                                                                 \
  } while (0)
#define DBG_DEREGISTER(o)                                                                                              \
  do {                                                                                                                 \
  } while (0)
#endif

} // namespace TerminalLibrary

#endif // __TERMINAL_DEBUG