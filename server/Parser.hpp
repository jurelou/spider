#ifndef                                             PARSER_HPP_
# define                                            PARSER_HPP_

#include                                            <vector>
#include                                            <string>
#include                                            <map>
#include                                            <boost/enable_shared_from_this.hpp>
#include                                            <boost/array.hpp>

class                                               Parser : public boost::enable_shared_from_this<Parser> {
  typedef                                           void (Parser::*Actions)();

  public:
    enum                                            resType {
      CONNECT,
      NO_RES,
      MESSAGE,
      MOUSE,
      STATUS,
    };
    Parser();
    ~Parser();
    static boost::shared_ptr<Parser>                create();
    void                                            makeMessageResponse();
    void                                            eventConnect();
    void                                            eventLogMouse();
    void                                            eventLogKey();
    void                                            eventStatus();
    void                                            manageEvent();
    const std::string&                              getResponse();
    void                                            receivePropsFromSocket(const std::string& str);
    resType                                         responseType;
  private:
    std::vector<std::tuple<std::string, Actions> >  actions;
    std::vector<std::string>                        getter;
    std::map<int, std::string>                      modifier;
    std::stringstream                               group;
    std::string                                     res;
};

#endif                                              /* !PARSER_HPP_ */
