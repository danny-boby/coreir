#ifndef INSTRUCTIONPROFILING_HPP_
#define INSTRUCTIONPROFILING_HPP_

#include "coreir.h"
#include <map>

namespace CoreIR {
  namespace Passes {

    class InstructionProfiling : public InstanceGraphPass {
      std::unordered_map<std::string, std::unordered_map<uint, int> > instruction_counts;

   public:
      static std::string ID;

      bool runOnInstanceGraphNode(InstanceGraphNode& node) override;
      void increment(Instance* inst);
      void writeToStream(std::ostream& os);
      void setAnalysisInfo() override {
        //addDependency("coreirjson");
      }

   InstructionProfiling() : InstanceGraphPass(ID,"Counts instances of each operator and bitwidth",
                                              /*is_analysis="*/true) {}


    };

  }
}
#endif
