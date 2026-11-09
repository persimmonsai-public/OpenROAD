#pragma once
#include <array>
#include <cstdint>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "dpl/Opendp.h"
#include "infrastructure/Coordinates.h"
#include "odb/geom.h"
#include "utl/Logger.h"

namespace odb {
class dbBlock;
class dbMaster;
class dbTech;
class dbOrientType;
}  // namespace odb

namespace dpl {
class Grid;
class Node;
class Padding;

struct EdgeSpacingEntry
{
  EdgeSpacingEntry(const int spc_in,
                   const bool is_exact_in,
                   const bool except_abutted_in)
      : spc(spc_in), is_exact(is_exact_in), except_abutted(except_abutted_in)
  {
  }
  bool operator<(const EdgeSpacingEntry& rhs) const { return spc < rhs.spc; }
  int spc;
  bool is_exact;
  bool except_abutted;
};

class PlacementDRC
{
 public:
  // Constructor
  PlacementDRC(utl::Logger* logger,
               Grid* grid,
               odb::dbTech* tech,
               Padding* padding,
               bool disallow_one_site_gap);
  ~PlacementDRC();
  void initFixedSupplyVias(odb::dbBlock* block);
  void clearFixedSupplyVias();
  bool checkEdgeSpacing(const Node* cell) const;
  // Check edge spacing for a cell at a given location and orientation
  bool checkEdgeSpacing(const Node* cell,
                        GridX x,
                        GridY y,
                        const odb::dbOrientType& orient) const;
  bool checkBlockedLayers(const Node* cell) const;
  bool checkBlockedLayers(const Node* cell,
                          GridX x,
                          GridY y,
                          const odb::dbOrientType& orient) const;
  bool checkFixedSupplyVias(const Node* cell) const;
  bool checkFixedSupplyVias(const Node* cell,
                            GridX x,
                            GridY y,
                            const odb::dbOrientType& orient) const;


  // Precise power-grid mode (opt-in, detailed_placement -precise_pg or
  // DPL_PRECISE_PG=1). The pixel mask stays as a conservative pre-filter;
  // when it reports a possible conflict, legality is decided from the
  // instance's pin and obstruction rectangles on each blocked layer against
  // the special-wire rectangles on that layer bloated by the layer's minimum
  // spacing. initPrecisePg() must be called after the grid is built.
  void initPrecisePg(odb::dbBlock* block);
  void clearPrecisePg();
  bool isPrecisePg() const { return precise_pg_; }
  // Check shared padding spacing conflicts
  bool checkPadding(const Node* cell) const;
  bool checkPadding(const Node* cell, GridX x, GridY y) const;

  // Check one site gap
  bool checkOneSiteGap(const Node* cell) const;
  bool checkOneSiteGap(const Node* cell, GridX x, GridY y) const;

  // aggregate function to check against all DRC types
  bool checkDRC(const Node* cell) const;
  bool checkDRC(const Node* cell,
                GridX x,
                GridY y,
                const odb::dbOrientType& orient) const;

  // Count the number of DRC check categories that fail (0-5)
  int countDRCViolations(const Node* cell) const;
  int countDRCViolations(const Node* cell,
                         GridX x,
                         GridY y,
                         const odb::dbOrientType& orient,
                         bool include_fixed_supply_vias = true) const;

  int getEdgeTypeIdx(const std::string& edge_type) const;
  bool hasCellEdgeSpacingTable() const;
  int getMaxSpacing(int edge_type_idx) const;

 private:
  class FixedSupplyVias;

  // Member variables
  utl::Logger* logger_{nullptr};
  Grid* grid_{nullptr};        // Pointer to the grid for placement
  Padding* padding_{nullptr};  // Pointer to the padding
  std::unordered_map<std::string, int> edge_types_indices_;
  std::vector<std::vector<EdgeSpacingEntry>>
      edge_spacing_table_;  // LEF58_CELLEDGESPACINGTABLE between edge type
                            // pairs [from_idx][to_idx]
  bool disallow_one_site_gap_{false};
  std::unique_ptr<FixedSupplyVias> fixed_supply_vias_;

  // Precise power-grid mode state.
  static constexpr int kMaxPgLevel = 8;  // routing levels kept in a uint8_t
  struct PgLayer
  {
    std::string name;
    int spacing = 0;  // LEF minimum spacing (DBU)
    int max_dx = 0;   // widest bloated rect, for the x range query
    std::vector<odb::Rect> rects;  // bloated by spacing, sorted by xMin
  };
  struct MasterPgShapes
  {
    odb::Rect boundary;  // placement boundary in master coordinates
    uint8_t mask = 0;    // levels with shapes
    std::array<std::vector<odb::Rect>, kMaxPgLevel> rects;  // pins + OBS
  };
  bool precise_pg_{false};
  uint8_t pg_mask_{0};  // levels that carry blocking special wires
  int pg_margin_sites_{0};
  std::array<PgLayer, kMaxPgLevel> pg_layers_;
  std::unordered_map<const odb::dbMaster*, MasterPgShapes> pg_masters_;
  size_t pg_last_logged_count_{0};

  bool checkBlockedLayersPixels(const Node* cell,
                                GridX x,
                                GridY y,
                                const odb::dbOrientType& orient,
                                bool check_wire_mask) const;
  bool checkBlockedLayersPrecise(const Node* cell,
                                 GridX x,
                                 GridY y,
                                 const odb::dbOrientType& orient) const;
  void addMasterPgShapes(odb::dbMaster* master);

  // Helper functions
  DbuX gridToDbu(GridX grid_x, DbuX site_width) const;
  void makeCellEdgeSpacingTable(odb::dbTech* tech);
  bool hasPaddingConflict(const Node* cell, const Node* padding_cell) const;
};

}  // namespace dpl
