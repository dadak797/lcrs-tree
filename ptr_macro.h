#pragma once

// Generate smart pointer typedefs for a class
// UPtr = std::unique_ptr
// Ptr = std::shared_ptr
// WPtr = std::weak_ptr
#define DECLARE_PTR(ClassName)                        \
class ClassName;                                      \
using ClassName ## UPtr = std::unique_ptr<ClassName>; \
using ClassName ## Ptr = std::shared_ptr<ClassName>;  \
using ClassName ## WPtr = std::weak_ptr<ClassName>;