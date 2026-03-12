#pragma once
#include <memory>

// std::atomic<shared_ptr> is not available in c++17 so we use a macro to detect the compiler version
#if __has_include(<version>)
    #include <version>
#endif

// Atomic shared_ptr support
#if defined(__cpp_lib_atomic_shared_ptr) && __cpp_lib_atomic_shared_ptr >= 201711L
    #include <atomic>
    #define HAS_ATOMIC_SHARED_PTR 1 // C++20 -> atomic (lock free, real time)
#else
    #include <mutex>
    #define HAS_ATOMIC_SHARED_PTR 0 //C++17 -> mutex (slower, not for audio real time)
#endif

// provide a uniform interface for atomic operations
// C++20 : load() is lock-free std::atomic<std::shared_ptr<T>> (real-time safe)
// C++17 : load() uses a mutex - NEVER call from audio thread!

// ==================== VERSION ATOMIQUE ====================

#if HAS_ATOMIC_SHARED_PTR
template <typename T>
class AtomicSharedPtr final
{
public:
    AtomicSharedPtr() noexcept = default; // empty ASP holding nullptr

    explicit AtomicSharedPtr(std::shared_ptr<T> p) noexcept : ptr{std::move(p)} {}

    AtomicSharedPtr(const AtomicSharedPtr&) = delete; // copy constructor is removed atmoic shouldnt be copyable
    AtomicSharedPtr& operator=(const AtomicSharedPtr&) = delete; // copy assignement is also removed

    // called from the audio thread
    // acquire means see the modifcations from the other threads
    [[nodiscard]] std::shared_ptr<T> load() const noexcept { return ptr.load(std::memory_order_acquire); }

    // called from the gui thread
    // release means the external threads can now see it
    void store(std::shared_ptr<T> p) noexcept { ptr.store(std::move(p), std::memory_order_release); }

    // replace the value return the old one
    [[nodiscard]] std::shared_ptr<T> exchange(std::shared_ptr<T> newPtr) noexcept
    {
        return ptr.exchange(std::move(newPtr), std::memory_order_acq_rel);
    }

    // for debug
    bool is_lock_free() const noexcept { return ptr.is_lock_free(); }

private:
    std::atomic<std::shared_ptr<T>> ptr{};
};

#else
// ==================== VERSION FALLBACK ====================

template <typename T>
class AtomicSharedPtr final
{
public:
    AtomicSharedPtr() noexcept = default;
    explicit AtomicSharedPtr(std::shared_ptr<T> p) noexcept : ptr(std::move(p)) {}

    AtomicSharedPtr(const AtomicSharedPtr&) = delete;
    AtomicSharedPtr& operator=(const AtomicSharedPtr&) = delete;

    // called by tge audio thread
    [[nodiscard]] std::shared_ptr<T> load() const noexcept
    {
        std::lock_guard<std::mutex> lock(mtx); // latch the mutex before reading
        return ptr;
    }

    // called by the gui thread
    void store(std::shared_ptr<T> newPtr) noexcept
    {
        std::lock_guard<std::mutex> lock(mtx); // latch the mutex before writing
        ptr = std::move(newPtr);
    }

    [[nodiscard]] std::shared_ptr<T> exchange(std::shared_ptr<T> newPtr) noexcept
    {
        std::lock_guard<std::mutex> lock(mtx);
        auto old = std::move(ptr);
        ptr = std::move(newPtr);
        return old;
    }

private:
    mutable std::mutex mtx; // protect the access of the variable
    std::shared_ptr<T> ptr{}; // protecred shared ptr
};
#endif