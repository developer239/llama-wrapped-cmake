#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

struct LlamaToken {
  // TODO: this should most likely be llama_token
  int tokenId;

  explicit LlamaToken(int id = 0) : tokenId(id) {}
};

struct ModelParams {
  int nGpuLayers = 0;
  bool vocabularyOnly = false;
  bool useMemoryMapping = true;
  bool useModelLock = false;
};

struct ContextParams {
  size_t nContext = 4096;
  int nThreads = 6;
  int nBatch = 512;
};

struct SamplingParams {
  size_t maxTokens = 1000;
  float temperature = 0.8f;
  int32_t topK = 45;
  float topP = 0.95f;
  float repeatPenalty = 1.1f;
  float frequencyPenalty = 0.0f;
  float presencePenalty = 0.0f;
  std::vector<LlamaToken> repeatPenaltyTokens;
};

class LlamaWrapper {
 public:
  LlamaWrapper();
  ~LlamaWrapper();

  LlamaWrapper(const LlamaWrapper&) = delete;
  LlamaWrapper& operator=(const LlamaWrapper&) = delete;

  LlamaWrapper(LlamaWrapper&&) noexcept = default;
  LlamaWrapper& operator=(LlamaWrapper&&) noexcept = default;

  bool InitializeModel(const std::string& modelPath, const ModelParams& params);

  bool InitializeContext(const ContextParams& params);

  [[nodiscard]] std::vector<LlamaToken> Encode(
      const std::string& text, bool addBos = true
  ) const;

  [[nodiscard]] std::string Decode(const std::vector<LlamaToken>& tokens) const;

  [[nodiscard]] LlamaToken TokenBos() const;
  [[nodiscard]] LlamaToken TokenEos() const;
  [[nodiscard]] LlamaToken TokenNl() const;

  void RunQueryStream(
      const std::string& systemPrompt, const std::string& userMessage,
      const SamplingParams& params,
      const std::function<void(const std::string&)>& callback
  ) const;

 private:
  class Impl;
  std::unique_ptr<Impl> pimpl;
};
